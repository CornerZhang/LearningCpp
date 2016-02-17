#include "NetSessionCenter.h"
#include <iostream>
#include <algorithm>

// net_session
net_session::state net_session::get_state() const {
    // ....
    
    return state::be_request;
}

const std::vector<char>& net_session::get_receive() const {
    return receive_data;
}

net_session::net_session() : be_used(false), send_data(nullptr), receive_data(), state_value(state::be_request) {
    
}

void net_session::send() {
    // network, send data from tcp socket
    // ...
    
    std::cout<<"be send: "<<send_data<<std::endl;
}



// net_session_center
net_session_center* ne_center = net_session_center::_PrivateInstall();

net_session_center* net_session_center::_PrivateInstall() {
    static net_session_center net_session_center_instance;
    return &net_session_center_instance;
}

// ctor - default
net_session_center::net_session_center() : be_run(true), go_thread( &net_session_center::go, this ), sessions( { net_session() } ) {
    go_thread.detach();
}

bool net_session_center::initialize(unsigned int timeout_msec) {
    sessions_lock lock(go_m);
    const unsigned long c = sessions.size();
    std::cout<<"count of sessions: "<<c<<std::endl;
    return true;
}

void net_session_center::shutdown() {
    be_run = false;
}

void net_session_center::go() {
    while (be_run) {
        sessions_lock lock(go_m);

        for (auto& session : sessions) {
            if (!session.be_used) {
                continue;
            }
            
            session.send();
            session.be_used = false;
        }
    }
}

const net_session* net_session_center::build_session(const char* send_data) {
    static auto condition = [](const net_session& s) -> bool {
        return !s.be_used;
    };
    
    sessions_lock lock(go_m);

    session_seq::iterator found = std::find_if(sessions.begin(), sessions.end(), condition);
    if (found==sessions.end()) {    // full, be error!!!
        return nullptr;
    }
    
    found->send_data = send_data;
    found->state_value = net_session::state::be_request;
    found->receive_data.clear();
    found->be_used = true;
    
    return &(*found);
}