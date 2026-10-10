/*******************************************************************************
 * @file        netinter.c
 * @brief       Network interface abstraction layer for multi-protocol support.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"
#include "ipc.h"

/*============================================================================*/
/*                         DEFINES & MACROS & ENUMS                           */
/*============================================================================*/
/* None */

/*============================================================================*/
/*                            FUNCTION PROTOTYPES                             */
/*============================================================================*/
/* None */

/*============================================================================*/
/*                          GLOBAL / STATIC VARIABLES                         */
/*============================================================================*/
/* None */

/*============================================================================*/
/*                           FUNCTION DEFINITIONS                             */
/*============================================================================*/

/*
 * Initialize all network interfaces.
 *
 * Parameters:
 *   pstGlobal - Pointer to the global archive structure.
 *
 * Returns:
 *   ERROR_CODE indicating success or failure.
 */
ERROR_CODE initialize_interfaces( GLOBAL_ARCHIVE* pstGlobal ) {

    DBG_ENTRY
    // Initializing LAN networks
#ifdef ENABLE_LORA_FOR_LAN
    if (initialize_LoRa() != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */

#ifdef ENABLE_IPC_SIMULATION
    if (initialize_ipc() != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */
    return ERR_OK_INTR;
}


/*
 * Send data to the Master.
 *
 * Parameters:
 *   pvData - Pointer to the data to be sent.
 *   u16Len - Length of the data in bytes.
 *
 * Returns:
 *   ERROR_CODE indicating success or failure.
 */
ERROR_CODE hal_send_to_master( PVOID pvData, UINT16 u16Len ) {
    DBG_ENTRY
#ifdef ENABLE_LORA_FOR_LAN
    if (send_LoRa(pvData, u16Len) != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */
    
#ifdef ENABLE_IPC_SIMULATION
    if (send_ipc(pvData, u16Len) != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */
    DBG_EXIT
    return ERR_OK_INTR;
}

/*
 * Receive data from the Master.
 *
 * Parameters:
 *   ppvData - Pointer to a pointer where the received data will be stored.
 *
 * Returns:
 *   ERROR_CODE indicating success or failure.
 */
ERROR_CODE hal_receive_from_master( PVOID *ppvData ) {
    DBG_ENTRY

#ifdef ENABLE_LORA_FOR_LAN
    if (receive_LoRa(ppvData) != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */

#ifdef ENABLE_IPC_SIMULATION
    if (receive_ipc(ppvData) != ERR_OK_INTR) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */

    DBG_EXIT
    return ERR_OK_INTR;
}
