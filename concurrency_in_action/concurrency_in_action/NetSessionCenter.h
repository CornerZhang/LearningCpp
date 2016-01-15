//
//  NetEventCenter.hpp
//  concurrency_in_action
//
//  Created by huazhang on 1/15/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#ifndef NetEventCenter_hpp
#define NetEventCenter_hpp

#include <thread>

enum class session_state {
    be_request,
    be_responsed
};

class net_session {
    friend class net_session_center;
public:
    session_state get_state() const;
    
    void        set_send_data( const char* data );
    const char* get_receive() const;
protected:
    session_state state;
    const char* send_data;
    const char* receive_data;
};

class net_session_center {
public:
    bool initialize(unsigned int timeout_sec);
    void shutdown();
    
    
    net_session* create_session();
    
    static net_session_center* _PrivateInstall();
protected:
    net_session_center();

    static void go();

    bool        be_run;
    std::thread go_thread;
};

extern net_session_center* ne_center;

#endif /* NetEventCenter_hpp */
