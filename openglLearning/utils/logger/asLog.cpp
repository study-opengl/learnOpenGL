//
//  asLog.cpp
//  createWindow
//
//  Created by Ashoka on 2018/4/26.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define __WRAP_FUNCTION__
#include "asLog.h"

#ifdef DEBUG

int as_debug(const char *time, const char *file, const char *func, int line, const char *format, ...)
{
    int done=0;
    va_list arg;
    va_start(arg, format);
    done = fprintf(stdout, "%s : +%s : %d :==> ", time, func, line);
    done += vfprintf(stdout, format, arg);
    printf("\n");
    va_end(arg);
    
    return done;
}
#endif

