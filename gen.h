#ifndef GEN_H
#define GEN_H

/* ================== INCLUDES ================== */

#include "options.h"
#include "netinter.h"

/* ================== TYPEDEFS ================== */

typedef int INT32;
typedef unsigned int UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef char INT8;
typedef unsigned char UINT8;
typedef void* PVOID;
typedef char* PCHAR;

/* ================== ENUMS ================== */

typedef enum { 
    ERR_OK = 0, 
    ERR_NET_IF_FAIL,
    ERR_INVALID_PARAM,
    ERR_INVALID_STATE,
    ERR_MEMORY_ALLOCATION_FAIL,
    ERR_INTEGRITY_CHECK_FAILED,
    ERR_ORDER_NOT_FR_SELF,
    ERR_STALE_ORDER,
    ERR_UNKNOWN
} ERROR_CODE;

typedef enum { 
    SLAVE_STOPPED = 0, 
    SLAVE_READY,
    SLAVE_INPROGRESS,
    SLAVE_TRANSITING,
    SLAVE_LISTENING,
    SLAVE_ORDER_PROCESSING,
    SLAVE_ORDER_PROCESSED
} SLAVE_STATUS;

/* ================== DEFINE ================== */

#define DBG_ENTRY print_dbg("%s:Entry", __FUNCTION__);
#define DBG_EXIT print_dbg("%s:Exit", __FUNCTION__); 

/* ================== STRUCTURES ================== */

typedef struct {

    SLAVE_STATUS enSlaveState;
    INT32 i32LANRecWaitTimeout;
    PVOID pvCurrentOrder;
    ERROR_CODE enInstErrStatus;
    UINT32 u32CurrSequence;

    /* Order Data */
    UINT32 u32VaulveNumber;
    UINT8 u8ActionType;
    UINT32 u32TimerCntS;
    UINT8 bIsTimerRunning;

    #ifdef ENABLE_LORA_FOR_LAN
    LAN_LORA_STATUS enLanLoraStatus;
#endif /* ENABLE_LORA_FOR_LAN */

#ifdef ENABLE_WIFI_FOR_WAN
    WAN_WIFI_STATUS enWanWiFiStatus;
#endif /* ENABLE_WIFI_FOR_WAN */

#ifdef ENABLE_GSM_FOR_WAN
    WAN_GSM_STATUS enWanGSMStatus;
#endif /* ENABLE_GSM_FOR_WAN */
}GLOBAL_ARCHIVE;

/* ================== FUNCTION PROTOTYPES ================== */

static inline void print_dbg( const char* argv, ... );
static inline void print_err( const char* argv, ... );
static inline void print_info( const char* argv, ... );
static inline void set_error ( PVOID pvInstance, ERROR_CODE enCurrentErr );
static inline ERROR_CODE get_error ( PVOID pvInstance );
static inline PCHAR convert_err2str( ERROR_CODE enErrorCode );

/* ================== INLINE FUNCTIONS ================== */
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

static inline void set_error ( PVOID pvInstance, ERROR_CODE enCurrentErr )
{
    ( pvInstance && (GLOBAL_ARCHIVE*)pvInstance->enInstErrStatus == ERR_OK ) ? ((*(GLOBAL_ARCHIVE)pvInstance)->enInstErrStatus = enCurrentErr ) : nullptr;
    return;
}

static inline ERROR_CODE get_error ( PVOID pvInstance )
{
    return (pvInstance ? ((*(GLOBAL_ARCHIVE*)pvInstance)->enInstErrStatus ) : 0);
}

static inline PCHAR convert_err2str( ERROR_CODE enErrorCode )
{
    switch ( enErrorCode )
    {
        case ERR_OK: return "ERR_OK";
        case ERR_NET_IF_FAIL: return "ERR_NET_IF_FAIL";
        case ERR_INVALID_PARAM: return "ERR_INVALID_PARAM";
        case ERR_INVALID_STATE: return "ERR_INVALID_STATE";
        case ERR_MEMORY_ALLOCATION_FAIL: return "ERR_MEMORY_ALLOCATION_FAIL";
        case ERR_INTEGRITY_CHECK_FAILED: return "ERR_INTEGRITY_CHECK_FAILED";
        case ERR_ORDER_NOT_FR_SELF: return "ERR_ORDER_NOT_FR_SELF";
        case ERR_UNKNOWN: return "ERR_UNKNOWN";
        default: return "ERR_UNKNOWN";
    }
}
#endif /* GEN_H */
