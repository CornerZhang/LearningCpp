//
//  chap_02.cpp
//  concurrency_in_action
//
//  Created by huazhang on 1/14/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "chap_02.hpp"
#include <thread>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include <mutex>

namespace {
    std::mutex m;
}

// List 2.1
struct func {
    int& i;
    
    func(int& i_): i(i_) {}
    
    void operator() (std::string& arg) {
        std::lock_guard<std::mutex> lock(m);

        arg = "be changed in func";
    }
};

class thread_guard {
    std::thread& t;
public:
    explicit thread_guard(std::thread& t_) : t(t_) {}
    
    ~thread_guard() {
        if (t.joinable()) {
            t.join();
        }
    }
    
    thread_guard(thread_guard const& ) = delete;
    thread_guard& operator= (thread_guard const& ) = delete;
};

// List 2.8, like code
void thread_in_vector() {
    const unsigned int max_threads = std::thread::hardware_concurrency() - 1;
    std::vector<std::thread> threads( max_threads );
    
    auto parallel_do = []() {
        std::lock_guard<std::mutex> lock(m);

        std::cout<<"show me\n";
    };
    
    for (unsigned int i=0; i<threads.size(); ++i) {
        threads[i] = std::thread( parallel_do );
    }
    
    // join all
    for (auto& t : threads)
        t.join();
}

extern std::thread::id mainThreadId;
void some_core_port_of_algorithm() {
    if (std::this_thread::get_id() == mainThreadId) {
        //...
    }
}

using thread_pools = std::unordered_map<std::thread::id, std::thread>;

void test_chap02() {
    thread_in_vector();
    
    std::string str("origin");
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func, std::ref(str));  // reference pass argument
    thread_guard g(t);
    
    t.join();
    //...
    
    std::cout<<str<<std::endl;
}