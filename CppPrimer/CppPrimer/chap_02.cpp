//
//  chap_02.cpp
//  CppPrimer
//
//  Created by huazhang on 10/30/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_02.hpp"

extern double pi = 3.1416;  // "extern" be ignore

constexpr int size() {
    return 16;
}

auto value = 4;
auto * data = "dafasf";

void Test_chap_02() {
    // 2.2 变量
    int units_sold = 0;
    int units_aod = {0};
    int units_cod(0);
    int units_god{0};
    
    //extern double Pi_over_2 = 1.57; // "compile error"
    double v = pi;
    
    char16_t ch = 'd';
    
    // 2.3.1 引用
    int i = 1024, i2 = 2048;
    int & r = i, r2 = i2;
    int i3 = 1024, & ri = i3;
    int & r3 = i3, & r4 = i2;
    
    // 2.4.4 constexpr和常量表达式
    constexpr int mf =20;
    constexpr int limit = mf + 1;
    constexpr double sz = size();
    constexpr int* pInt = nullptr;
    auto * pa = pInt;
    const auto f = 4.5f;
    auto & g = r3;
    
    // 2.5.1 类型别名
    typedef double wages_t;
    using int64_u = long long int;
    
    // 2.5.3 decltype类型指示符
    decltype(size()) sum = sz;
    decltype(*pa) aas = i3; // decltype's result: auto& = int&
    decltype((i)) as = i2;  // decltype's result: int&, because (i) gain a reference to int
}