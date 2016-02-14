#ifndef _ALLOCATOR_DEF_H_
#define _ALLOCATOR_DEF_H_

namespace inf {
    // C++ 11, need Heap class be implemented
    template <typename type>
    class allocator {
    public:
        using value_type = type;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using reference = value_type&;
        using const_reference = const value_type&;
        using size_type = size_t;
        
        template <typename U>
        using other = allocator<U>;
        
        allocator() noexcept {}
        
        allocator(const allocator& ) noexcept {}
        
        template <typename U>
        allocator(const allocator<U>& ) noexcept {
            
        }
        
        ~allocator() {}
        
        value_type* allocate( std::size_t num ) {
            return reinterpret_cast<value_type*>( std::malloc( num*sizeof(value_type) ) );
        }
        
        void deallocate(value_type* p, std::size_t num) {
            std::free( p );
        }
        
        template <typename U, typename... Args>
        void construct(U* p, Args&&... args) {
            ::new(p) U( std::forward<Args>(args)... );
        }
        
        template <typename U>
        void destroy(U* p) {
            p->~U();
        }
        
        pointer address(reference a) const noexcept {
            return &a;
        }
        
        const_pointer address(const_reference a) const noexcept {
            return &a;
        }
        
        size_type max_size() const noexcept {
            return size_type(-1) / sizeof(value_type);
        }
    };
    
    template <>
    class allocator<void> {
    public:
        using value_type = void;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        
        template <typename U>
        struct rebind {
            using other = allocator<U>;
        };
    };
    
    template <typename T1, typename T2>
    bool operator == (const allocator<T1>& a, const allocator<T2>& b) noexcept {
        return (&a == &b);
    }
    
    template <typename T1, typename T2>
    bool operator != (const allocator<T1>& a, const allocator<T2>& b) noexcept {
        return !(a==b);
    }
}   // ::inf

#endif /* _ALLOCATOR_DEF_H_ */
