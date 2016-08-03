//
//  TestLitdef.hpp
//  DemoCpp
//
//  Created by huazhang on 8/3/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#ifndef TestLitdef_hpp
#define TestLitdef_hpp

template <typename T>
struct lit_demo;

template <>
struct lit_demo<int> {
    static const int One;
};

void Test_litdef();

#endif /* TestLitdef_hpp */
