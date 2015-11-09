//
//  chap_06.cpp
//  CppPrimer
//
//  Created by huazhang on 11/3/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_06.hpp"
#include <utility>
#include <string>

constexpr int new_sz() {
    return 42;
}

constexpr size_t scale(size_t cnt) {
    return new_sz() * cnt;
}

typedef int (*call_1_t)(int a);
typedef float (*call_2_t)(int f);

std::string::size_type sumLength(const std::string& , const std::string& ) {
    return 0;
}

std::string::size_type largeLength(const std::string& , const std::string& ) {
    return 0;
}

// decltype(sumLength)* 就是sumLength函数的类型的指针
decltype(sumLength)* getFcn(const std::string& ) {
    return sumLength;
}

void Test_chap_06() {
    int f = new_sz();
    const int g = new_sz();
    constexpr int t = new_sz();
    
    // constexpr function 不一定返回常量表达式
    int arr[scale(2)];
    int n = 2;
    int a2[scale(n)];
    int a3 = scale(n);  // 返回变量

}