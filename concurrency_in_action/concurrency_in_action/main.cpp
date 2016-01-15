//
//  main.cpp
//  concurrency_in_action
//
//  Created by huazhang on 1/14/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "chap_01.hpp"
#include "chap_02.hpp"
#include "chap_03.hpp"
#include "NetSessionCenter.h"
#include <thread>

void foo() {
    
}

unsigned int max_concurrency = 0;
std::thread::id mainThreadId;

const char* data = "dafdasfasdfasfasfasfd";

int main(int argc, const char * argv[]) {
    const unsigned int wait_time_out_msec = 300;
    ne_center->initialize(wait_time_out_msec);
    
    const net_session* session_a = ne_center->build_session( data );
    
    while (true) {
        if( session_a->get_state() == net_session::state::be_request ) {
            // ...
            break;
        }
        
    }
    
    max_concurrency = std::thread::hardware_concurrency();
    mainThreadId = std::this_thread::get_id();
    
    
    test_chap01();
    test_chap02();
    test_chap03();
    
    ne_center->shutdown();
    return 0;
}
