//
//  chap_10.cpp
//  CppPrimer
//
//  Created by huazhang on 11/5/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_10.hpp"

struct Object {
    
    //static auto lambda = [this]()->int { return 0; };
private:
    int a, b;
};

void Test_10() {
    int var = 10;
    float fs = 34.0f;
    
    auto f = [var](int e) -> int { return 36*var*e; }; // lambda expression
    auto g = [&]{ var = 23; };
    auto z = [=]{ return var * 10; };
    auto xg = [&, fs] { return fs * var; };
    auto xt = [=, &fs]()->float { return fs * var; };

    int r = f(34);
    g();
    r = z();
    float fr = xg();
    fr = xt();
    
    Object object;
    
}