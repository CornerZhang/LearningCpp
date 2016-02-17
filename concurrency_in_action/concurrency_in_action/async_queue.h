#ifndef execute_queue_hpp
#define execute_queue_hpp

#include <thread>
#include <vector>

namespace aq {
    typedef void (*function_t)(void* context);
    
    class async_queue {
    public:
        async_queue();
        
        bool initialize();
        void shutdown();
        
        void dispatch(function_t f, void* context = nullptr);
        
    private:
        void execute();
        
        
        using mutex_lock = std::lock_guard<std::mutex>;

        struct function_ent_t {
            function_t  f;
            void*       context;
            bool        unused;
        };

        using queut_t = std::vector<function_ent_t>;

        bool        q_run;
        queut_t     queue;
        std::mutex  q_mutex;
        std::thread q_thread;
    };
}   // ::aq

#endif /* execute_queue_hpp */
