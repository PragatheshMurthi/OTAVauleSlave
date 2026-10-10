/*******************************************************************************
 * @file        lorahal.h
 * @brief       LoRa HAL (Hardware Abstraction Layer) interface header.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef LORAHAL_H
#define LORAHAL_H

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                           FUNCTION PROTOTYPES                              */
/*============================================================================*/
/**
 * @brief Initializes LoRa communication interface.
 * @return ERROR_CODE ERR_OK_INTR on success, error code on failure
 */
ERROR_CODE initialize_LoRa( void );

/**
 * @brief Sends data via LoRa interface.
 * @param [in] pvData Pointer to data buffer to send
 * @param [in] u16Len Length of data in bytes
 * @return ERROR_CODE ERR_OK_INTR on success, error code on failure
 */
ERROR_CODE send_LoRa( PVOID pvData, UINT16 u16Len );

/**
 * @brief Receives data via LoRa interface.
 * @param [out] ppvData Pointer to receive buffer pointer
 * @return ERROR_CODE ERR_OK_INTR on success, error code on failure
 */
ERROR_CODE receive_LoRa( PVOID *ppvData );

#ifdef __cplusplus
}
#endif

#endif /* LORAHAL_H */