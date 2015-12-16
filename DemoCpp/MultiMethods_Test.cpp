#include "MultiMethods_Test.h"
#include "MultiMethods.h"
#include <iostream>

struct square;
struct rect;
struct circle;

using shape_visit_compositor = inf::visit_compositor<square, rect, circle>;

struct shape {
    virtual ~shape() = default;
    virtual void accept(shape_visit_compositor &) const = 0;
};
struct rect : shape {
    virtual void accept(shape_visit_compositor &v) const override { v.visit(*this); }
};

struct square : rect {
    virtual void accept(shape_visit_compositor &v) const override { v.visit(*this); }
};
struct circle : shape {
    virtual void accept(shape_visit_compositor &v) const override { v.visit(*this); }
};

class shape_printer : public shape_visit_compositor
{
public:
    void visit(const rect &s) override { std::cout << "rect"; }
    void visit(const square &s) override { std::cout << "square"; }
    void visit(const circle &s) override { std::cout << "circle"; }
};

struct printer
{
    void operator() (const rect &s) const { std::cout << "rect"; }
    void operator() (const square &s) const { std::cout << "square"; }
    void operator() (const circle &s) const { std::cout << "circle"; }
};

struct p3
{
    template <typename S1, typename S2, typename S3>
    void operator() (const S1& s1, const S2& s2, const S3& s3)
    {
        printer()(s1); std::cout << " ";
        printer()(s2); std::cout << " ";
        printer()(s3); std::cout << std::endl;
    }
};


struct is_equal {
    bool operator()(shape &s1, shape &s2) const
    {
        shape_printer printer;
        s1.accept(printer);
        std::cout << " != ";
        s2.accept(printer);
        std::cout << std::endl;
        return false;
    }

    template <typename S>
    bool operator()(S &s1, S &s2) const
    {
        shape_printer printer;
        s1.accept(printer);
        std::cout << " == ";
        s2.accept(printer);
        std::cout << std::endl;
        return true;
    }
};

void Test_MultiMethod()
{
    rect rect;
    square sq;
    circle c;
    shape *shapes[] = { &rect, &sq, &c };

    using namespace inf;
    
    for (auto shape1 : shapes) {
        for (auto shape2 : shapes) {
            dispatch<bool, shape_visit_compositor>(is_equal(), *shape1, *shape2);
        }
    }
    
    dispatch<void, shape_visit_compositor>(printer(), sq);
    dispatch<void, shape_visit_compositor>(printer(), *shapes[0]);

    dispatch<bool, shape_visit_compositor>(is_equal(), rect, *shapes[0]);
    dispatch<bool, shape_visit_compositor>(is_equal(), *shapes[0], rect);

    dispatch<bool, shape_visit_compositor>(is_equal(), rect, sq);

    dispatch<void, shape_visit_compositor>(p3(), c, rect, sq);

}

