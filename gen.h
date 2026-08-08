#ifndef GEN_H
#define GEN_H

#include "options.h"

typedef int INT32;
typedef unsigned int UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef char INT8;
typedef unsigned char UINT8;

typedef enum { 
    ERR_OK = 0, 
    ERR_NET_IF_FAIL = 1
} ERROR_CODE;

static inline void print_dbg( const char* argv, ... ) {
    va_list args;
    int done;

    // 2. Initialize the argument list
    va_start(args, format);
    done = vprintf(format, args);
    va_end(args);

    return done;
}

static inline void print_err( const char* argv, ... ) {
    va_list args;
    int done;

    // 2. Initialize the argument list
    va_start(args, format);
    done = vprintf(format, args);
    va_end(args);

    return done;
}

static inline void print_info( const char* argv, ... ) {
    va_list args;
    int done;

    // 2. Initialize the argument list
    va_start(args, format);
    done = vprintf(format, args);
    va_end(args);

    return done;
}
#endif /* GEN_H */
