//
//  TestLitdef.cpp
//  DemoCpp
//
//  Created by huazhang on 8/3/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "TestLitdef.h"
#include <litdef.h>

const int lit_demo<int>::One = 1;

void Test_litdef() {
    int i = lit_demo<int>::One;
}
