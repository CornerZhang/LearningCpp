//
//  UsingConstructor.cpp
//  ModernCpp
//
//  Created by CornerZhang on 14-10-15.
//  Copyright (c) 2014年 NeXtreme.com. All rights reserved.
//

// chap 3.1

#include "testUsingConstructor.h"
#include <iostream>

struct Base {
    Base(int i) {
        
    }
    
    void print(double v) {
        std::cout<<"double "<<v<<std::endl;
    }
};

struct Drived: public Base {

//    Drived(int i) : Base(i) {	// 不需要
//        
//    }
    using Base::Base;	// declare constructor to here scope
    using Base::print;
    
    void print(int v) {
        std::cout<<"int "<<v<<std::endl;
    }
private:
    int value{0};	// 设置默认值为0
};

void testUsingConstructor() {
    Base object(1);
    Drived ob = 1;
    
    object.print(4.5);	// double 4.5
    ob.print(4.5);		// double 4.5
}