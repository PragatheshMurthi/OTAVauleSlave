/* Simple simulated HAL implementations for LoRa/WiFi/GSM */
#include "lorahal.h"
#include "comm.h"
#include <stdlib.h>
#include <string.h>

ERROR_CODE initialize_LoRa( void ) {
    print_info("initializeLoRa: simulated\n");
    return ERR_OK;
}

ERROR_CODE send_LoRa( PVOID pvData, UINT16 u16Len ) {
    (void)pvData;(void)u16Len;
    print_info("send_LoRa: simulated send\n");
    return ERR_OK;
}

ERROR_CODE receive_LoRa( PVOID *ppvData ) {
    // Simulate receiving an ORDER_BUFFER
    print_info("receive_LoRa: simulated receive\n");
    return ERR_OK;
}

