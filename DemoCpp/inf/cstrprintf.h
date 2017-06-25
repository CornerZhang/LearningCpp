//
//  cstrprintf.h
//  DemoCpp
//
//  Created by CornerZhang on 6/25/17.
//  Copyright © 2017 huazhang. All rights reserved.
//

#ifndef cstrprintf_h
#define cstrprintf_h

#include <cstdarg>

namespace inf {
    int snPrintf( char *dest, int size, const char *fmt, ...);
    int snPrintf( wchar_t *dest, int size, const wchar_t *fmt, ...);
    int vsnPrintf( char *dest, int size, const char *fmt, va_list argptr );
    int vsnPrintf( wchar_t *dest, int size, const wchar_t *fmt, va_list argptr );
}   // ::inf


#endif /* cstrprintf_h */
