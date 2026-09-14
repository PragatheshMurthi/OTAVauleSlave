/*******************************************************************************
 * @file        comm.h
 * @brief       Communication module header - order processing and CRC validation.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef __COMM__
#define __COMM__

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"

/*============================================================================*/
/*                           STRUCTURES & UNIONS                              */
/*============================================================================*/
/**
 * @struct ORDER_BUFFER
 * @brief Structure representing an order from the master with metadata and payload.
 */
typedef struct {
    /* Order metadata */
    UINT32 u32OrderLength;
    UINT32 u32OrderCRC;

    /* Data starts */
    UINT32 u32ValveNumber;
    UINT32 u32OrderSequenceNumber;
    UINT8 u8ActionType;
    UINT8 u8Reserved[3];
    UINT32 u32TimerCntS;
    
} ORDER_BUFFER;

/**
 * @struct ACK_BUFFER
 * @brief Structure representing acknowledgment from slave to master.
 */
typedef struct {
    /* Order metadata */
    UINT32 u32OrderLength;
    UINT32 u32OrderCRC;

    /* Data starts */
    UINT32 u32SequenceNumber;
    UINT32 u32ValveNumber;
    char acOrderStatus[MAX_ORDER_RESULT_LENGTH];
    
} ACK_BUFFER;

/*============================================================================*/
/*                           FUNCTION PROTOTYPES                              */
/*============================================================================*/
/**
 * @brief Expects and receives an order from the network master.
 * @param [in] pstGlobalArchive Pointer to global archive structure
 * @return PVOID Pointer to received order buffer, or NULL on error
 * @note Buffer pointer must remain valid for subsequent processing
 */
PVOID expect_order ( GLOBAL_ARCHIVE* pstGlobalArchive );

/**
 * @brief Parses received order and updates global archive with order details.
 * @param [in,out] pstGlobalArchive Pointer to global archive structure
 * @param [in] pvOrderBuffer Pointer to received order buffer
 * @return void
 * @note Sets error status on failure
 */
VOID parse_order( GLOBAL_ARCHIVE* pstGlobalArchive, PVOID pvOrderBuffer );

/**
 * @brief Fills acknowledgment buffer with current status information.
 * @param [out] pstAckBuffer Pointer to acknowledgment buffer to fill
 * @param [in] pstGlobalArchive Pointer to global archive structure
 * @return ERROR_CODE Status of operation
 */
ERROR_CODE fill_ack_buffer( ACK_BUFFER* pstAckBuffer, GLOBAL_ARCHIVE* pstGlobalArchive );

/**
 * @brief Evaluates integrity of received order using CRC validation.
 * @param [in] pvOrderBuffer Pointer to order buffer to validate
 * @return ERROR_CODE ERR_OK if integrity check passes, error code otherwise
 */
ERROR_CODE evaluate_integrity( PVOID pvOrderBuffer );

/**
 * @brief Calculates CRC-32 checksum for data validation.
 * @param [in] pvOrderBuffer Pointer to buffer to calculate CRC for
 * @param [in] u16OrderLength Length of data in buffer
 * @return UINT32 Calculated CRC-32 value
 */
UINT32 calculate_crc( PVOID pvOrderBuffer, UINT16 u16OrderLength );

/**
 * @brief Updates master with acknowledgment of order processing status.
 * @param [in] pstGlobalArchive Pointer to global archive structure
 * @return void
 */
VOID update_master( GLOBAL_ARCHIVE* pstGlobalArchive );

#endif  /* __COMM__ */