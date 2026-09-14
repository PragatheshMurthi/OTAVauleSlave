/*******************************************************************************
 * @file        netinter.h
 * @brief       Network interface abstraction layer header.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef NETINTER_H
#define NETINTER_H

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
/* enums moved to gen.h */
#include "gen.h"

/*============================================================================*/
/*                           FUNCTION PROTOTYPES                              */
/*============================================================================*/
/**
 * @brief Initializes all configured network interfaces (IPC, LoRa, etc.).
 * @param [in,out] pstGlobal Pointer to global archive structure
 * @return ERROR_CODE ERR_OK if all interfaces initialized, error code on failure
 */
ERROR_CODE initialize_interfaces( GLOBAL_ARCHIVE* pstGlobal );

/**
 * @brief Sends data to master using configured interface.
 * @param [in] pvData Pointer to data buffer to send
 * @param [in] u16Len Length of data in bytes
 * @return ERROR_CODE ERR_OK on success, error code on failure
 */
ERROR_CODE hal_send_to_master( PVOID pvData, UINT16 u16Len );

/**
 * @brief Receives data from master using configured interface.
 * @param [out] ppvData Pointer to receive buffer pointer
 * @return ERROR_CODE ERR_OK on success, error code on failure
 */
ERROR_CODE hal_receive_from_master( PVOID *ppvData );

#endif /* NETINTER_H */
