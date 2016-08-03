#include "precompiled.h"
#pragma hdrstop

namespace inf {

        /*
        =============
        SysLock::create
        =============
        */
        void SysLock::init( lockHandle_t& handle ) {
            ::InitializeCriticalSection( &handle );
        }

        /*
        =============
        SysLock::destroy
        =============
        */
        void SysLock::destroy( lockHandle_t& handle ) {
            ::DeleteCriticalSection( &handle );
        }

        /*
        =============
        SysLock::acquire
        =============
        */
        bool SysLock::acquire( lockHandle_t& handle, bool blocking ) {
            if ( ::TryEnterCriticalSection( &handle ) == 0 ) {
                if ( !blocking ) {
                    return false;
                }
                ::EnterCriticalSection( &handle );
            }
            return true;
        }

        /*
        =============
        SysLock::release
        =============
        */
        void SysLock::release( lockHandle_t& handle ) {
            ::LeaveCriticalSection( &handle );
        }

}   // ::inf
