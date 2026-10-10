/*******************************************************************************
 * @file        valve.h
 * @brief       Valve control and actuation interface header.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

#ifndef VALVE_H
#define VALVE_H

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"

/*============================================================================*/
/*                           FUNCTION PROTOTYPES                              */
/*============================================================================*/
/**
 * @brief Initiates execution of a received valve control order.
 * @param [in,out] pstGlobalArchive Pointer to global archive structure
 * @return void
 * @note Sets error status on failure
 */
VOID initiate_order( GLOBAL_ARCHIVE* pstGlobalArchive );

/**
 * @brief Hardware abstraction for valve actuation with simulated delay.
 * @param [in] u8ActionType Type of action to perform (open/close/etc.)
 * @return ERROR_CODE ERR_OK_INTR on success, error code on failure
 * @note Includes random delay of 0-5 seconds for realistic simulation
 */
ERROR_CODE hal_actuate_valve( UINT8 u8ActionType );

#endif  /* VALVE_H */