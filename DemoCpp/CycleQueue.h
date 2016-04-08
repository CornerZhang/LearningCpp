#ifndef _CYCLE_QUEUE_
#define _CYCLE_QUEUE_

#include <iostream>

template <typename Type>
class cycle_queue {
public:
    cycle_queue() {
        
    }
    
    void    initialize(int count) {
        if (empty()) {
            reallocate(count);
        }
    }
    
    void    reallocate(int count) {
        if (data) {
            delete [] data;
        }
        
        allocated_count = count;
        data = new Type[allocated_count];
        used_count = 0;
        front_index = 0;
        rear_index = 0;
    }
    
    bool    empty() const {
        return used_count == 0;
    }
    
    int     count_allocated() const {
        return allocated_count;
    }
    
    int     count() const {
        return used_count;
    }
    
    void    append(const Type& value) {
        if (queue_fulled()) {
            rear_index = rebuild_index(rear_index);
            front_index = rebuild_index(front_index);
            data[rear_index] = value;
            
            ++rear_index;
            ++front_index;
            
        }else{  // !fulled & front_index == 0
            rear_index = rebuild_index(rear_index);
            data[rear_index] = value;
            
            ++rear_index;
        }
        
        increase_used();
    }
    
    const Type&   front() const {
        return data[rebuild_index(front_index)];
    }
    
    Type&   front() {
        return data[rebuild_index(front_index)];
    }
    
    const Type& operator [] (const int index) const {
        // 0 <= index < used_count
        assert( index < used_count );
        assert( index >= 0 );
        
        int real_index = rebuild_index(front_index + index);
        
        return data[real_index];
    }
    
    Type& operator [] (const int index) {
        // 0 <= index < used_count
        assert( index < used_count );
        assert( index >= 0 );
        
        int real_index = rebuild_index(front_index + index);
        
        return data[real_index];
    }
    
    typedef void (*callback_t) (const Type& value);
    
    void print_debug(callback_t caller) const {
        for (int i=0; i<used_count; ++i) {
            caller(data[i]);
        }
        std::cout<<std::endl;
    }
private:
    int     rebuild_index(int index) const {
        return index % allocated_count;
    }
    
    bool    queue_fulled() const {
        return used_count == allocated_count;
    }
    
    void    increase_used() {
        if (queue_fulled())
            return;
        ++used_count;
    }
    
    Type*   data;
    int     allocated_count;
    int     used_count;
    
    int     front_index;
    int     rear_index;
};

#endif // _CYCLE_QUEUE_