//
//  asLog.h
//  createWindow
//
//  Created by Ashoka on 2018/4/16.
//

#ifndef asLog_h
#define asLog_h

#ifdef DEBUG

int as_debug(const char *time, const char *file, const char *func, int line, const char * format, ...);
#define asLog(...) as_debug(__TIME__, __FILE__, __func__, __LINE__, __VA_ARGS__)
#else /* IF DEBUG NOT DEFINED*/
#define asLog(...) /* DO NOT PRINT ANYTHING IF DEBUG IS NOT PRESENT */
#endif /* END OF DEBUG */

#endif /* asLog_h */
