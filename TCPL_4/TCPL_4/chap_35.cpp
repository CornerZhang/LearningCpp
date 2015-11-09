//
//  chap_35.cpp
//  TCPL_4
//
//  Created by huazhang on 11/4/15.
//  Copyright © 2015 huazhang. All rights reserved.
//

#include "chap_35.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;

void Test_35() {
    steady_clock::time_point t = steady_clock::now();
    
    const int timeSecond = 4;
    sleep(timeSecond);
    
    steady_clock::duration d = steady_clock::now() - t;
    
    long long unit = duration_cast<milliseconds>(d).count();
    std::cout<<"something took "<<unit<<" ms"<<std::endl;
}