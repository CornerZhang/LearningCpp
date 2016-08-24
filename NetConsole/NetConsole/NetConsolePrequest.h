//
//  NetConsolePrequest.h
//  NetConsole
//
//  Created by Cyberzei on 8/24/16.
//  Copyright © 2016 Cyberzei. All rights reserved.
//

#ifndef NetConsolePrequest_h
#define NetConsolePrequest_h


#include "PlatformConfig.h"

// define for ssize_t
#if defined(_MSC_VER) || defined(__MINGW32__)
#   include <BaseTsd.h>
#   include <WinSock2.h>

#   ifndef __SSIZE_T
#       define __SSIZE_T
typedef SSIZE_T ssize_t;
#   endif // __SSIZE_T
#else
#   include <sys/select.h>
#endif

// define for socket io
#if defined(_MSC_VER) || defined(__MINGW32__)
#   include <io.h>
#   include <WS2tcpip.h>
#   include <Winsock2.h>
#   if defined(__MINGW32__)
#       include "platform/win32/inet_pton_mingw.h"
#   endif
#   define bzero(a, b) memset(a, 0, b);
#   if (NC_TARGET_PLATFORM == NC_PLATFORM_WINRT)
#       include "platform/winrt/inet_ntop_winrt.h"
#       include "platform/winrt/inet_pton_winrt.h"
#       include "platform/winrt/CCWinRTUtils.h"
#   endif
#else
#   include <netdb.h>
#   include <unistd.h>
#   include <arpa/inet.h>
#   include <netinet/in.h>
#   include <sys/socket.h>
#   include <sys/un.h>
#   include <sys/ioctl.h>
#endif

/** @def NC_FORMAT_PRINTF(formatPos, argPos)
 * Only certain compiler support __attribute__((format))
 *
 * @param formatPos 1-based position of format string argument.
 * @param argPos    1-based position of first format-dependent argument.
 */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#   define NC_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute)
#   if __has_attribute(format)
#       define NC_FORMAT_PRINTF(formatPos, argPos) __attribute__((__format__(printf, formatPos, argPos)))
#   endif // __has_attribute(format)
#else
#   define NC_FORMAT_PRINTF(formatPos, argPos)
#endif

#endif /* NetConsolePrequest_h */
