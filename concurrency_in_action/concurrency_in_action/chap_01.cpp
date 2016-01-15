//
//  chap_01.cpp
//  concurrency_in_action
//
//  Created by huazhang on 1/14/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "chap_01.hpp"
#include <thread>
#include <iostream>
#include <functional>
#include <mutex>

namespace {
    std::mutex m;
}

// #1
void thread_hello(int id, float dent) {
    std::lock_guard<std::mutex> lock(m);
    std::cout<<"Hello Concurrent World, "<<id<<", "<<dent<<"\n";
}

// #2
//std::function<void()> func_in( thread_hello );
std::function<void(int id, float dent)> func_in;

// #3
struct my_function {
    void operator() () {
        std::lock_guard<std::mutex> lock(m);
        std::cout<<"my function\n";
    }
};

// #4
auto lambda = [] {
    thread_hello(14, 0.6f);
};

// #5
struct my_object {
    void todo( int i, std::string& name ) {
        
    }
};

void test_chap01() {
    std::thread t(thread_hello, 2, 4.5f);    // #1

    my_function my_func;
    std::thread tfm(my_func);       // #3
    
    func_in = thread_hello;
//    func_in = my_func;
    std::thread tf(func_in, 23, 3.0f);        // #2
    tf.detach();    // joinable == false

    std::thread tfl(lambda);        // #4
    
    my_object myObj;
    std::string str("dfaf");
    std::thread tfo(&my_object::todo, &myObj, 16, std::ref(str));   // #5
    
    tfo.join();
    tfl.join();
    tfm.join();
    if (tf.joinable()) {
        tf.join();
    }
    t.join();
}