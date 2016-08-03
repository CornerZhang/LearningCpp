// Copyright (C) 2007 Id Software, Inc.
//

#include "SysLock.h"
#include <errno.h>

namespace inf {
/*
=============
SysLock::Create
=============
*/
void SysLock::init( lockHandle_t& handle ) {
	pthread_mutexattr_t	attr;

	pthread_mutexattr_init( &attr );
	pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK );
	pthread_mutex_init( &handle, &attr );
	pthread_mutexattr_destroy( &attr );	
}

/*
=============
SysLock::Destroy
=============
*/
void SysLock::destroy( lockHandle_t& handle ) {
	pthread_mutex_destroy( &handle );
}

/*
=============
SysLock::Acquire
=============
*/
bool SysLock::acquire( lockHandle_t& handle, bool blocking ) {
	if ( blocking ) {
		pthread_mutex_lock( &handle );
		return true;
	}
    
	if ( pthread_mutex_trylock( &handle ) == EBUSY ) {  // EBUSY in <errno.h>
		return false;
	}
	return true;
}

/*
=============
SysLock::Release
=============
*/
void SysLock::release( lockHandle_t& handle ) {
	pthread_mutex_unlock( &handle );
}
    
}   // ::inf
