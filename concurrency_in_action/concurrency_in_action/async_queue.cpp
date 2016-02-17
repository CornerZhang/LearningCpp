#include "async_queue.h"
#include <iostream>

namespace aq {
    async_queue::async_queue() : q_thread(), queue() {
    }
        
    bool async_queue::initialize() {
        q_run = true;
        q_thread = std::thread(&async_queue::execute, this);
        q_thread.detach();
        
        return true;
    }
    
    void async_queue::dispatch(function_t f, void* context) {
        static auto exist_empty_func = [](const function_ent_t& s) -> bool {
            return s.unused;
        };

        mutex_lock lock(q_mutex);
        
        queut_t::iterator found = std::find_if(queue.begin(), queue.end(), exist_empty_func);
        if (found != queue.end()) {
            // reuse exist the empty
            found->f = f;
            found->context = context;
            found->unused = false;
        }else{
            // create new to tail
            function_ent_t fent = { f, nullptr, false };
            queue.push_back(fent);
        }
    }
    
    void async_queue::shutdown() {
        q_run = false;

        if (q_thread.joinable()) {
            q_thread.join();
        }
        
    }
    
    void async_queue::execute() {
        while (q_run) {
            mutex_lock lock(q_mutex);

            for (auto& f : queue) {
                if (f.unused || !f.f) {
                    continue;
                }
                f.f(f.context);
                f.unused = true;
            }
        }
    }
}   // ::aq