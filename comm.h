#ifndef __COMM__
#define __COMM__

/* ================== INCLUDES ================== */
#include "gen.h"

/* ================== STRUCTURES ================== */
typedef struct {
    /* Order metadata */
    UINT32 u32OrderLength;
    UINT32 u32OrderCRC;

    /* Data starts */
    UINT32 u32VaulveNumber;
    UINT32 u32OrderSequenceNumber;
    UINT8 u8ActionType;
    UINT8 u8Reserved[3];
    UINT32 u32TimerCntS;
    
} ORDER_BUFFER;

typedef struct {
    /* Order metadata */
    UINT32 u32OrderLength;
    UINT32 u32OrderCRC;

    /* Data starts */
    UINT32 u32SequenceNumber;
    UINT32 u32ValveNumber;
    CHAR acOrderStatus[MAX_ORDER_RESULT_LENGTH];
    
} ACK_BUFFER;

/* ================== FUNCTION PROTOTYPES ================== */

PVOID expect_order ( GLOBAL_ARCHIVE* pstGlobalArchive );
VOID parse_order( GLOBAL_ARCHIVE* pstGlobalArchive, PVOID pvOrderBuffer );

ERROR_CODE evaluate_integrity( PVOID pvOrderBuffer );
UINT32 calculate_crc( PVOID pvOrderBuffer, UINT16 u16OrderLength );

#endif  /* __COMM__ */