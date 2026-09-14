/*******************************************************************************
 * @file        ipc.h
 * @brief       IPC (Inter-Process Communication) interface header.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef IPC_H
#define IPC_H

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"

/*============================================================================*/
/*                           FUNCTION PROTOTYPES                              */
/*============================================================================*/
/**
 * @brief Initializes IPC socket and binds to slave receive port.
 * @return ERROR_CODE ERR_OK on success, error code on failure
 */
ERROR_CODE initialize_ipc( void );

/**
 * @brief Sends data to master via IPC unicast.
 * @param [in] pvData Pointer to data buffer to send
 * @param [in] u16Len Length of data in bytes
 * @return ERROR_CODE ERR_OK on success, error code on failure
 * @note Master address is 127.0.0.1:46000
 */
ERROR_CODE send_ipc( PVOID pvData, UINT16 u16Len );

/**
 * @brief Receives data from master via IPC broadcast.
 * @param [out] ppvData Pointer to receive buffer pointer
 * @return ERROR_CODE ERR_OK on success, error code on failure
 * @note Slave listens on 255.255.255.255:47001
 */
ERROR_CODE receive_ipc( PVOID *ppvData );

#endif /* IPC_H */
