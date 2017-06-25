//
//  cstrfunc.cpp
//  DemoCpp
//
//  Created by CornerZhang on 6/25/17.
//  Copyright © 2017 huazhang. All rights reserved.
//

#include "cstrprintf.h"
#include "litdef.h"

#include <cwchar>


int inf::snPrintf( char *dest, int size, const char *fmt, ...) {
    int ret;
    va_list argptr;
    
    va_start( argptr, fmt );
    // must enable C11 compiler
    ret = vsnprintf( dest, size, fmt, argptr );
    va_end( argptr );
    
    dest[size-1] = inf::c_def<char>::zero;
    if ( ret < 0 || ret >= size ) {
        return -1;
    }
    return ret;
}

int inf::snPrintf( wchar_t *dest, int size, const wchar_t *fmt, ...) {
    int ret;
    va_list argptr;
    
    va_start( argptr, fmt );
    // must enable C11 compiler
    ret = vswprintf( dest, size, fmt, argptr );
    va_end( argptr );
    
    dest[size-1] = inf::c_def<wchar_t>::zero;
    if ( ret < 0 || ret >= size ) {
        return -1;
    }
    return ret;
}

int inf::vsnPrintf( char *dest, int size, const char *fmt, va_list argptr ) {
    int ret;
    
    ret = vsnprintf( dest, size, fmt, argptr );
    
    dest[size-1] = inf::c_def<char>::zero;
    if ( ret < 0 || ret >= size ) {
        return -1;
    }
    return ret;
}

int inf::vsnPrintf( wchar_t *dest, int size, const wchar_t *fmt, va_list argptr ) {
    int ret;
    
    ret = vswprintf( dest, size, fmt, argptr );
    
    dest[size-1] = inf::c_def<wchar_t>::zero;
    if ( ret < 0 || ret >= size ) {
        return -1;
    }
    return ret;
}
