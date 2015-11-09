//
//  chap_19.cpp
//  CppPrimer
//
//  Created by huazhang on 11/9/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_19.hpp"
#include <utility>
#include <initializer_list>
#include <iostream>

enum struct color_channel { // : int (default)
    red,
    green,
    blue,
    alpha
};

void function1(std::initializer_list<int> list) {
    for (auto e : list) {
        std::cout<<e<<std::endl;
    }
}

void Test_19() {
    int ev = static_cast<int>(color_channel::blue);
    int a = std::move(ev);
    
    function1( {23,12,34} );
}