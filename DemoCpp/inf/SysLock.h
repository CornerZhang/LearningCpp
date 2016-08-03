#ifndef __SYSLOCK_H__
#define __SYSLOCK_H__

#ifndef _WIN32
#include <pthread.h>
#endif

#define lockHandle_t	pthread_mutex_t

namespace inf {

        class SysLock {
        public:
            static void				init( lockHandle_t& handle );
            static void				destroy( lockHandle_t& handle );
            static bool				acquire( lockHandle_t& handle, bool blocking );
            static void				release( lockHandle_t& handle );

        private:
            SysLock( void ) {}
        };


}   // ::inf

#endif /* !__SYSLOCK_H__ */
