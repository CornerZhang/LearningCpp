//
//  main.cpp
//  InsideCpp11_code
//
//  Created by CornerZhang on 14-10-14.
//  Copyright (c) 2014年 NeXtreme.com. All rights reserved.
//

#include "EnumValue.h"
#include "TestSTDC.h"
#include "testUsingConstructor.h"

int cmp( int a, int b )
{
    return (a<b) - (a>b);
}

int main(int argc, const char * argv[]) {
    TestEnumValue();
    TestSTDC_version();
    testUsingConstructor();
    
    int r = cmp(3, 8);
    
    return 0;
}
