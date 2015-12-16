//
//  TestUnion.cpp
//  InsideCpp11_code
//
//  Created by huazhang on 11/9/15.
//  Copyright © 2015 NeXtreme.com. All rights reserved.
//

#include "TestUnion.hpp"
#include <string>

namespace {
    union TestDemo {
        static int a;
        float f;
        
        std::string s;
        
        static int GetValue() {
            return a;
        }
    public:
        TestDemo() : f(1) {
            
        }
    };
    
    int TestDemo::a;
}

void TestUnion() {
    TestDemo::a = 34;
    int r = TestDemo::GetValue();
}