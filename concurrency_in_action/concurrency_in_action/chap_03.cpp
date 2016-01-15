//
//  chap_03.cpp
//  concurrency_in_action
//
//  Created by huazhang on 1/15/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "chap_03.hpp"
#include <mutex>
#include <list>

std::list<int> some_list;
std::mutex mutex;

void add(int new_value) {
    mutex.lock();
    some_list.push_back(new_value);
    mutex.unlock();
}

void add_to_list(int new_value) {
    std::lock_guard<std::mutex> guard(mutex);
    
    some_list.push_back(new_value);
}

bool list_contains(int value_to_find) {
    std::lock_guard<std::mutex> guard(mutex);
    
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

void test_chap03() {
    
}