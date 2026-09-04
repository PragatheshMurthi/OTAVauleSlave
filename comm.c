/* ============= INCLUDES ============= */
#include "comm.h"

/* ============= Function Definitions ============= */


/* ============== RECEIVER IMPLEMENTATION ============== */

/*
 * Expects an order from the network and processes it.
 *
 * Parameters:
 *   pstGlobalArchive - Pointer to the global archive structure.
 * 
 */
PVOID expect_order ( GLOBAL_ARCHIVE* pstGlobalArchive )
{
    ERROR_CODE enErroCode = ERR_OK;
    PVOID pvOrderBuffer = NULL;

    DBG_ENTRY

    if ( NULL == pstGlobalArchive )
    {
        set_error(ERR_INVALID_PARAM);
        print_err("%s:GlobalArchive<KO>", __FUNCTION__);
        DBG_EXIT
        return NULL;
    }

    if ( get_error( pstGlobalArchive ) != ERR_OK )
    {
        print_err("%s:ErrorInPreviousState<KO><%d>", __FUNCTION__, get_error( pstGlobalArchive ));
        DBG_EXIT
        return NULL;
    }
    
    enErroCode = hal_receive( pvOrderBuffer );

    if ( enErrorCode == ERR_OK && NULL != pvOrderBuffer )
    {
        print_dbg("%s:OrderRecieved<OK><OB[%p]><%d>", __FUNCTION__, pvOrderBuffer, enErrorCode);
        enErrorCode = evaluate_integrity( pvOrderBuffer );

        if ( enErrorCode != ERR_OK )
        {
            // Handle integrity check failure
            print_err("%s:OrderIntegrityCheck<KO>ERR<%d>", __FUNCTION__, enErrorCode);
            set_error( enErrorCode );
        }
    } else {
        print_err("%s:OrderRecieve<KO>ERR<%d>", __FUNCTION__, enErrorCode);
        set_error( enErrorCode );
    }

    print_info("%s:OrderRecieved<OK><OB[%p]><%d>", __FUNCTION__, pvOrderBuffer, enErrorCode);

    DBG_EXIT
    return pvOrderBuffer;
}

/*
 * Evaluates the integrity of the received order using CRC.
 *
 * Parameters:
 *   pvOrderBuffer - Pointer to the order buffer.
 * Returns:
 *   ERROR_CODE - Status of the integrity check.
 */
ERROR_CODE evaluate_integrity( PVOID pvOrderBuffer )
{
    UINT32 u32CalculatedCRC = 0;
    UINT32 u32ReceivedCRC   = 0;
    UINT16 u16OrderLength   = 0;
]
    DBG_ENTRY

    if ( NULL == pvOrderBuffer )
    {
        print_err("%s:OrderBuffer<KO><NULL>", __FUNCTION__);
        set_error(ERR_INVALID_PARAM);
        DBG_EXIT
        return ERR_INVALID_PARAM;
    }

    u16OrderLength = ((ORDER_BUFFER*)pvOrderBuffer)->u32OrderLength;
    u32ReceivedCRC = ((ORDER_BUFFER*)pvOrderBuffer)->u32OrderCRC;

    print_dbg("%s:OrderIntegrityCheck<OK><OB[%p]OL[%d]RC[%d]>", __FUNCTION__, pvOrderBuffer, u16OrderLength, u32ReceivedCRC);

    u32CalculatedCRC = calculate_crc( pvOrderBuffer, u16OrderLength );

    if ( u32ReceivedCRC != u32CalculatedCRC )
    {
        print_err("%s:OrderIntegrityCheck<KO><OB[%p]OL[%d]RC[%d]CC[%d]>", __FUNCTION__, pvOrderBuffer, u16OrderLength, u32ReceivedCRC, u32CalculatedCRC);
        set_error(ERR_INTEGRITY_CHECK_FAILED);
        DBG_EXIT
        return ERR_INTEGRITY_CHECK_FAILED;
    }

    DBG_EXIT
    return ERR_OK;
}

/*
 * Calculates the CRC for the given order buffer.
 *
 * Parameters:
 *   pvOrderBuffer - Pointer to the order buffer.
 *   u16OrderLength - Length of the order.
 * Returns:
 *   UINT32 - Calculated CRC value.
 */
UINT32 calculate_crc( PVOID pvOrderBuffer, UINT16 u16OrderLength )
{
    // Note: ~0UL initializes the context to 0xFFFFFFFF
    // esp_rom_crc32_le calculates little-endian CRC-32 (standard Ethernet IEEE 802.3)
    return esp_rom_crc32_le(~0UL, pvOrderBuffer, u16OrderLength) ^ 0xFFFFFFFF;
}

/* ============== SEND IMPLEMENTATION ============== */

VOID update_master( GLOBAL_ARCHIVE* pstGlobalArchive )
{
    ERROR_CODE enErrorCode = ERR_OK;
    ACK_BUFFER stAckBuffer = { 0 };

    DBG_ENTRY

    if ( NULL == pstGlobalArchive )
    {
        set_error(pstGlobalArchive, ERR_INVALID_PARAM);
        print_err("%s:GlobalArchive<KO>", __FUNCTION__);
        DBG_EXIT
        return ERR_INVALID_PARAM;
    }

    if ( pstGlobalArchive->enSlaveState == SLAVE_ORDER_PROCESSED )
    {
        strncpy(&(stAckBuffer.acOrderStatus), "ORDER_PROCESSED", sizeof(stAckBuffer.acOrderStatus) - 1);
        stAckBuffer.acOrderStatus[sizeof(stAckBuffer.acOrderStatus) - 1] = '\0';
    } else {
        strncpy(&(stAckBuffer.acOrderStatus), convert_err2str(get_error(pstGlobalArchive)), sizeof(stAckBuffer.acOrderStatus) - 1);
        stAckBuffer.acOrderStatus[sizeof(stAckBuffer.acOrderStatus) - 1] = '\0';
    }

    if ( fill_ack_buffer( &stAckBuffer.acOrderStatus, sizeof(stAckBuffer.acOrderStatus), pstGlobalArchive ) != ERR_OK )
    {
        strncpy(&(stAckBuffer.acOrderStatus), "STATUS_POPULATION_FAILED", sizeof(stAckBuffer.acOrderStatus) - 1);
        stAckBuffer.acOrderStatus[sizeof(stAckBuffer.acOrderStatus) - 1] = '\0';
        print_err("%s:FailedToFillAckBuffer<KO>", __FUNCTION__);
    }

    stAckBuffer.u32OrderCRC = calculate_crc( &stAckBuffer, sizeof(stAckBuffer) );
    stAckBuffer.u32OrderLength = sizeof(stAckBuffer);
    
    enErrorCode = hal_send( &stAckBuffer, sizeof(stAckBuffer) );

    if ( enErrorCode == ERR_OK && NULL != &stAckBuffer )
    {
        print_dbg("%s:AckSent<OK><AB[%p]><%d>", __FUNCTION__, &stAckBuffer, enErrorCode);
    } else {
        print_err("%s:AckSend<KO>ERR<%d>", __FUNCTION__, enErrorCode);
    }

    DBG_EXIT
    return enErrorCode;
}

ERROR_CODE fill_ack_buffer( PCHAR pchAckStatus, UINT16 u16AckLength, GLOBAL_ARCHIVE* pstGlobalArchive )
{
    if ( NULL == pchAckStatus || NULL == pstGlobalArchive )
    {
        print_err("%s:AckBufferOrGlobalArchive<KO><AB[%p]GA[%p]>", __FUNCTION__, pchAckStatus, pstGlobalArchive);
        return ERR_INVALID_PARAM;
    }

    switch ( pstGlobalArchive->enInstErrStatus )
    {
        case ERR_OK:
            if ( pstGlobalArchive->enSlaveState == SLAVE_ORDER_PROCESSED )
            {
                strncpy(pchAckStatus, "ORDER_PROCESSED", u16AckLength - 1);
                pchAckStatus[u16AckLength - 1] = '\0';
            } else {
                strncpy(pchAckStatus, "ORDER_IN_PROGRESS", u16AckLength - 1);
                pchAckStatus[u16AckLength - 1] = '\0';
            }
            break;
        case ERR_NET_IF_FAIL:
        case ERR_INVALID_PARAM:
        case ERR_INVALID_STATE:
        case ERR_MEMORY_ALLOCATION_FAIL:
        case ERR_INTEGRITY_CHECK_FAILED:
        case ERR_ORDER_NOT_FR_SELF:
        case ERR_STALE_ORDER:
            strncpy(pchAckStatus, "STALE_ORDER", u16AckLength - 1);
            pchAckStatus[u16AckLength - 1] = '\0';  
        default:
            strncpy(&(pstAckBuffer->acOrderStatus), "STATUS_POPULATION_FAILED", u16AckLength - 1);
            pstAckBuffer->acOrderStatus[u16AckLength - 1] = '\0';
            break;
    }

    pstAckBuffer->u32SequenceNumber = pstGlobalArchive->u32CurrSequence;
    pstAckBuffer->u32ValveNumber = pstGlobalArchive->u32VaulveNumber;
    
    print_dbg("%s:AckBufferFilled<OK><AB[%p]VN[%d]SN[%d]AT[%d]TC[%d]>", __FUNCTION__, pstAckBuffer, pstAckBuffer->u32VaulveNumber, pstAckBuffer->u32OrderSequenceNumber, pstAckBuffer->u8ActionType, pstAckBuffer->u32TimerCntS);

    return ERR_OK;
}

/* ============== PARSER IMPLEMENTATION ============== */

/*
 * Parses the received order and updates the global archive with order details.
 *
 * Parameters:
 *  pstGlobalArchive - Pointer to the global archive structure.
 *  pvOrderBuffer - Pointer to the order buffer.
 */
VOID parse_order( GLOBAL_ARCHIVE* pstGlobalArchive, PVOID pvOrderBuffer )
{
    
    DBG_ENTRY
    
    if ( NULL == pstGlobalArchive || NULL == pvOrderBuffer )
    {
        set_error(pstGlobalArchive, ERR_INVALID_PARAM);
        print_err("%s:<KO-ADD><GA[%p]OB[%p]>", __FUNCTION__, pstGlobalArchive, pvOrderBuffer);
        DBG_EXIT
        return;
    }

    if ( get_error( pstGlobalArchive ) != ERR_OK )
    {
        print_err("%s:ErrorInPreviousState<KO><%d>", __FUNCTION__, get_error( pstGlobalArchive ));
        DBG_EXIT
        return;
    }

    /* Pre-condition to evaluate the self valve number */
    if ( SELF_VALVE_NUMBER != ((ORDER_BUFFER*)pvOrderBuffer)->u32VaulveNumber )
    {
        print_dbg("%s:OrderValveNumber<KO><OB[%p]VN[%d]CN[%d]>", __FUNCTION__, pvOrderBuffer, SELF_VALVE_NUMBER, ((ORDER_BUFFER*)pvOrderBuffer)->u32VaulveNumber);
        set_error(pstGlobalArchive, ERR_ORDER_NOT_FR_SELF);
        DBG_EXIT
        return;
    }

    /* Pre-condition to evaluate the Sequence number */
    if ( ((ORDER_BUFFER*)pvOrderBuffer)->u32OrderSequenceNumber <= pstGlobalArchive->u32CurrSequence )
    {
        print_dbg("%s:OrderSequenceNumber<KO><OB[%p]SN[%d]CN[%d]>", __FUNCTION__, pvOrderBuffer, ((ORDER_BUFFER*)pvOrderBuffer)->u32OrderSequenceNumber, pstGlobalArchive->u32CurrSequence);
        set_error(pstGlobalArchive, ERR_STALE_ORDER);
        DBG_EXIT
        return;
    }

    if ( NULL != pstGlobalArchive->pvCurrentOrder )
    {

        pstGlobalArchive->u32VaulveNumber   = ((ORDER_BUFFER*)pstGlobalArchive->pvCurrentOrder)->u32VaulveNumber;
        pstGlobalArchive->u8ActionType      = ((ORDER_BUFFER*)pstGlobalArchive->pvCurrentOrder)->u8ActionType;
        pstGlobalArchive->u32TimerCntS      = ((ORDER_BUFFER*)pstGlobalArchive->pvCurrentOrder)->u32TimerCntS;
        pstGlobalArchive->bIsTimerRunning    = (pstGlobalArchive->u32TimerCntS > 0) ? 1 : 0;
        pstGlobalArchive->u32CurrSequence    = ((ORDER_BUFFER*)pstGlobalArchive->pvCurrentOrder)->u32OrderSequenceNumber;

        print_dbg("%s:OrderParsed<OK><OB[%p]VN[%d]AT[%d]TC[%d]>", __FUNCTION__, pstGlobalArchive->pvCurrentOrder, pstGlobalArchive->u32VaulveNumber, pstGlobalArchive->u8ActionType, pstGlobalArchive->u32TimerCntS);

    }

    DBG_EXIT
    return;    
}