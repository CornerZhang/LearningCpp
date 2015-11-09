//
//  main.cpp
//  TCPL_4
//
//  Created by huazhang on 8/31/15.
//  Copyright (c) 2015 huazhang. All rights reserved.
//

#include <iostream>
#include "chap_35.hpp"

using size_tl = unsigned int;

void* gPtr = nullptr;

template <typename T>
struct DefineType {
    T value;
    
public:
    void Func();
};

int main(int argc, const char * argv[]) {
    auto value = 0;
    
    char* string = "dfasfasfasdfsadf";
    
    int ast = 2232141243;

    Test_35();
    
    return 0;
}
