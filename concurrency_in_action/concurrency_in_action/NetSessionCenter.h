#ifndef NetSessionCenter_hpp
#define NetSessionCenter_hpp

#include <thread>
#include <vector>
#include <array>

struct net_session {
    enum class state {
        be_request, // default
        be_responsed
    };
    
    state       get_state() const;
    
    const std::vector<char>& get_receive() const;
protected:
    friend class net_session_center;

    net_session();
    
    void send();
    
    state               state_value;
    const char*         send_data;
    std::vector<char>   receive_data;
    bool                be_used;
    // mutex
};

class net_session_center {
public:
    bool initialize(unsigned int timeout_sec);
    void shutdown();
    
    
    const net_session* build_session(const char* send_data);  // the new session state as default ( be_request )
    
    static net_session_center* _PrivateInstall();
protected:
    net_session_center();

    void    go();

    static constexpr unsigned int max_sessions_number = 16;
    using sessions_lock = std::lock_guard<std::mutex>;
    using session_seq = std::array<net_session, max_sessions_number>;
    
    session_seq sessions;
    
    bool        be_run;
    std::thread go_thread;
    std::mutex  go_m;
};

extern net_session_center* ne_center;

#endif /* NetSessionCenter_hpp */
