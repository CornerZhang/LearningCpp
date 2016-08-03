#include "Lock.h"

namespace inf {

    using inf::SysLock;

    /*
    =============
    Lock::Lock
    =============
    */
    Lock::Lock( void ) {
        SysLock::init( handle );
    }

    /*
    =============
    Lock:~:Lock
    =============
    */
    Lock::~Lock( void ) {
        SysLock::destroy( handle );
    }

    /*
    =============
    Lock::acquire
    =============
    */
    bool Lock::acquire( bool blocking ) {
        return SysLock::acquire( handle, blocking );
    }

    /*
    =============
    Lock::release
    =============
    */
    void Lock::release( void ) {
        SysLock::release( handle );
    }

}   // ::inf
