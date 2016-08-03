#ifndef __LOCK_H__
#define __LOCK_H__

#include <SysLock.h>

namespace inf {

    class Lock {
    public:
        Lock();
        ~Lock();

        bool				acquire( bool blocking = true );
        void				release();

#if (INF_PLATFORM != PLATFORM_WIN32) && (INF_PLATFORM != PLATFORM_WIN64)
        lockHandle_t*		get_handle( void ) const { return &handle; }
#endif

    protected:
        lockHandle_t		handle;
    };

    template< bool doLock >
    class ScopedLock {
    public:
    };

    template<>
    class ScopedLock< true > {
    public:
        ScopedLock( Lock& lock, bool blocking = true ) : lock( lock ) {
            lock.acquire( blocking );
        }
        ~ScopedLock( void ) {
            lock.release();
        }
    private:
        ScopedLock( const ScopedLock& rhs );
        ScopedLock& operator=( const ScopedLock& rhs );
        Lock& lock;
    };


    template<>
    class ScopedLock< false > {
    public:
        ScopedLock( Lock& , bool  = true ) {
        }
        ~ScopedLock( void ) {
        }
    private:
        ScopedLock( const ScopedLock& rhs );
        ScopedLock& operator=( const ScopedLock& rhs );
    };

}   // ::inf


#endif /* !__LOCK_H__ */
