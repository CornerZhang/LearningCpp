//
//  NetEventCenter.cpp
//  concurrency_in_action
//
//  Created by huazhang on 1/15/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "NetEventCenter.hpp"
#include <iostream>

net_session_center* ne_center = net_session_center::_PrivateInstall();

net_session_center* net_session_center::_PrivateInstall() {
    static net_session_center net_session_center_instance;
    return &net_session_center_instance;
}

net_session_center::net_session_center() : be_run(true), go_thread( go ) {
    go_thread.detach();
}

bool net_session_center::initialize(unsigned int timeout_msec) {
    // ...
    return true;
}

void net_session_center::shutdown() {
    be_run = false;
}

void net_session_center::go() {
    while (ne_center->be_run) {
        //....
        std::cout<<"be run\n";
    }
}