//
//  sys_mac.cpp
//  DemoCpp
//
//  Created by huazhang on 7/21/16.
//  Copyright © 2016 huazhang. All rights reserved.
//

#include "sys_mac.h"
#include <stdio.h>
#include <stdarg.h>
#include <CoreFoundation/CoreFoundation.h>

#define INF_MAX_STRING_CHARS 1024

void fatal_error( const char* fmt, ...) {
    va_list		argptr;
    char		text[INF_MAX_STRING_CHARS];
    
    va_start( argptr, fmt );
    vsnprintf( text, INF_MAX_STRING_CHARS, fmt, argptr );
    va_end( argptr );
    
    const int unused = 0;
    MessageBox("fatal_error", text, unused);
}

void MessageBox(const char* header, const char* message, int type)
{
    CFStringRef header_ref = CFStringCreateWithCString(NULL,header, (unsigned int)strlen(header));
    CFStringRef message_ref = CFStringCreateWithCString(NULL,message, (unsigned int)strlen(message));
    
    CFOptionFlags result;
    
    CFUserNotificationDisplayAlert(
                                   0,
                                   kCFUserNotificationNoteAlertLevel,
                                   NULL,
                                   NULL,
                                   NULL,
                                   header_ref,
                                   message_ref,
                                   NULL,
                                   CFSTR("Cancel"),
                                   NULL,
                                   &result
                                   );
    
    CFRelease(header_ref);
    CFRelease(message_ref);
}