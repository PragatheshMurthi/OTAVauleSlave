#include "gen.h"
#include "ipc.h"

ERROR_CODE initialize_interfaces( GLOBAL_ARCHIVE* pstGlobal ) {

    DBG_ENTRY
    // Initializing LAN networks
#ifdef ENABLE_LORA_FOR_LAN
    if (initialize_LoRa() != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */

#ifdef ENABLE_IPC_SIMULATION
    if (initialize_ipc() != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */
    return ERR_OK;
}

ERROR_CODE hal_send_to_master( PVOID pvData, UINT16 u16Len ) {
    DBG_ENTRY
#ifdef ENABLE_LORA_FOR_LAN
    if (send_LoRa(pvData, u16Len) != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */
    
#ifdef ENABLE_IPC_SIMULATION
    if (send_ipc(pvData, u16Len) != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */
    DBG_EXIT
    return ERR_OK;
}

ERROR_CODE hal_receive_from_master( PVOID *ppvData ) {
    DBG_ENTRY

#ifdef ENABLE_LORA_FOR_LAN
    if (receive_LoRa(ppvData) != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_LORA_FOR_LAN */

#ifdef ENABLE_IPC_SIMULATION
    if (receive_ipc(ppvData) != ERR_OK) {
        DBG_EXIT
        return ERR_NET_IF_FAIL;
    }
#endif /* ENABLE_IPC_SIMULATION */

    DBG_EXIT
    return ERR_NET_IF_FAIL;
}
