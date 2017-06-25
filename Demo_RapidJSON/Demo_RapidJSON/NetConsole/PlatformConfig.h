#ifndef PlatformConfig_h
#define PlatformConfig_h

// define supported target platform macro which CC uses.
#define NC_PLATFORM_UNKNOWN            0
#define NC_PLATFORM_IOS                1
#define NC_PLATFORM_ANDROID            2
#define NC_PLATFORM_WIN32              3
#define NC_PLATFORM_LINUX              5
#define NC_PLATFORM_MAC                8
#define NC_PLATFORM_WINRT             13

// Determine target platform by compile environment macro.
#define NC_TARGET_PLATFORM             NC_PLATFORM_UNKNOWN

// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // exclude android for binding generator.
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM         NC_PLATFORM_IOS
#elif TARGET_OS_MAC
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM         NC_PLATFORM_MAC
#endif
#endif

// android
#if defined(ANDROID)
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM         NC_PLATFORM_ANDROID
#endif

// win32
#if defined(_WIN32) && defined(_WINDOWS)
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM         NC_PLATFORM_WIN32
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM         NC_PLATFORM_LINUX
#endif

// WinRT (Windows 8.1 Store/Phone App)
#if defined(WINRT)
#undef  NC_TARGET_PLATFORM
#define NC_TARGET_PLATFORM          NC_PLATFORM_WINRT
#endif

#endif /* PlatformConfig_h */
