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
#include <iomanip>
#include <sstream>

using namespace std::chrono;

void Test_35() {
    std::ostringstream sstream;
    
    auto now = std::chrono::system_clock::now();

    auto p = duration_cast<milliseconds>( now.time_since_epoch() );
    sstream << "milliseconds since epoch: (chrono) :"<< p.count() << '\n';
    
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    sstream << "now, the time was "<< std::put_time(std::localtime(&now_c), "%F %T") << std::setfill('0') << std::setw(3)<< '\n';
    
    auto start = std::chrono::steady_clock::now();
    sstream << "Hello World\n";
    auto end = std::chrono::steady_clock::now();
    sstream << "Printing took "<< std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()<< "us.\n";
    
    std::cout<<sstream.str();
}