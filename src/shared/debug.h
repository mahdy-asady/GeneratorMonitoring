#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define LOG_LEVEL_NONE          0
#define LOG_LEVEL_ERROR         1
#define LOG_LEVEL_WARNING       2
#define LOG_LEVEL_INFO          3
#define LOG_LEVEL_TRACE         4

/*      TRACE   INFO    WARN    ERROR   NONE
TRACE   YES      NO      NO      NO      NO
INFO    YES      YES      NO      NO      NO
WARN    YES      YES      YES      NO      NO
ERROR   YES      YES      YES      YES      NO*/

/* -- Macro Definitions */
//#define debugFatal(M, ...) {}
#define debugFatal(M, ...)           do{debugError(M, ##__VA_ARGS__); while(1);}while(0)

#if LOG_LEVEL>= LOG_LEVEL_ERROR
    #define debugError(M, ...)           printf("[ERR] (%s:%d) " M "\n", __BASE_FILE__, __LINE__, ##__VA_ARGS__);
#else
    #define debugError(format, ...)      {}
#endif

#if LOG_LEVEL>= LOG_LEVEL_WARNING
    #define debugWarning(M, ...)         printf("[WARN] (%s:%d) " M "\n", __BASE_FILE__, __LINE__, ##__VA_ARGS__);
#else
    #define debugWarning(format, ...)    {}
#endif

#if LOG_LEVEL>= LOG_LEVEL_INFO
    #define debugInfo(M, ...)            printf("[INFO] (%s:%d) " M "\n", __BASE_FILE__, __LINE__, ##__VA_ARGS__);
    #define DebugBlock(x)               do {x} while(0)
#else
    #define debugInfo(format, ...)       {}
    #define DebugBlock(x)               {}
#endif

#if LOG_LEVEL>= LOG_LEVEL_TRACE
    #define debugTrace()                 printf("[..] (%s:%d->%s)\n", __BASE_FILE__, __LINE__, __func__);
#else
    #define debugTrace()                 {}
#endif

#endif /*__DEBUG_H__*/
