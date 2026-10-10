/*******************************************************************************
 * @file        gen.h
 * @brief       General definitions, typedefs, enums, and macros for the system.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef GEN_H
#define GEN_H

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include <stdarg.h>
#include <stdio.h>
#include "options.h"

/*============================================================================*/
/*                              TYPEDEFS & ENUMS                              */
/*============================================================================*/

typedef int INT32;
typedef unsigned int UINT32;
typedef short INT16;
typedef unsigned short UINT16;
typedef char INT8;
typedef unsigned char UINT8;
typedef void* PVOID;
typedef char* PCHAR;
typedef void VOID;

/* ================== ENUMS ================== */

typedef enum { 
    ERR_OK_INTR = 0,
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

/*============================================================================*/
/*                         DEFINES & MACROS & DEBUG                           */
/*============================================================================*/
#define DBG_ENTRY print_dbg("%s:Entry", __FUNCTION__);
#define DBG_EXIT print_dbg("%s:Exit", __FUNCTION__); 

/*============================================================================*/
/*                           STRUCTURES & UNIONS                              */
/*============================================================================*/

typedef struct {

    SLAVE_STATUS enSlaveState;
    INT32 i32LANRecWaitTimeout;
    PVOID pvCurrentOrder;
    ERROR_CODE enInstErrStatus;
    UINT32 u32CurrSequence;

    /* Order Data */
    UINT32 u32ValveNumber;
    UINT8 u8ActionType;
    UINT32 u32TimerCntS;
    UINT8 bIsTimerRunning;

#ifdef ENABLE_GSM_FOR_WAN
    WAN_GSM_STATUS enWanGSMStatus;
#endif /* ENABLE_GSM_FOR_WAN */
}GLOBAL_ARCHIVE;

/*============================================================================*/
/*                        INLINE FUNCTION PROTOTYPES                          */
/*============================================================================*/
/**
 * @brief Prints debug message with variable arguments to stdout.
 * @param format String format specifier
 * @param ... Variable argument list
 * @return void
 */
static inline void print_dbg( const char* format, ... );

/**
 * @brief Prints error message with variable arguments to stderr.
 * @param format String format specifier
 * @param ... Variable argument list
 * @return void
 */
static inline void print_err( const char* format, ... );

/**
 * @brief Prints informational message with variable arguments to stdout.
 * @param format String format specifier
 * @param ... Variable argument list
 * @return void
 */
static inline void print_info( const char* format, ... );

/**
 * @brief Sets error code in the global archive instance.
 * @param [in] pvInstance Pointer to GLOBAL_ARCHIVE instance
 * @param [in] enCurrentErr Error code to set
 * @return void
 */
static inline void set_error ( PVOID pvInstance, ERROR_CODE enCurrentErr );

/**
 * @brief Retrieves current error code from global archive instance.
 * @param [in] pvInstance Pointer to GLOBAL_ARCHIVE instance
 * @return ERROR_CODE Current error code
 */
static inline ERROR_CODE get_error ( PVOID pvInstance );

/**
 * @brief Converts error code to human-readable string.
 * @param [in] enErrorCode Error code to convert
 * @return PCHAR Pointer to error description string
 */
static inline PCHAR convert_err2str( ERROR_CODE enErrorCode );

/* ================== INLINE FUNCTIONS ================== */
static inline void print_dbg( const char* format, ... ) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

static inline void print_err( const char* format, ... ) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    printf("\n");
}

static inline void print_info( const char* format, ... ) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

static inline void set_error ( PVOID pvInstance, ERROR_CODE enCurrentErr )
{
    if ( pvInstance ) {
        ((GLOBAL_ARCHIVE*)pvInstance)->enInstErrStatus = enCurrentErr;
    }
}

static inline ERROR_CODE get_error ( PVOID pvInstance )
{
    if ( pvInstance )
        return ((GLOBAL_ARCHIVE*)pvInstance)->enInstErrStatus;
    return ERR_UNKNOWN;
}

static inline PCHAR convert_err2str( ERROR_CODE enErrorCode )
{
    switch ( enErrorCode )
    {
        case ERR_OK_INTR: return "ERR_OK_INTR";
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
