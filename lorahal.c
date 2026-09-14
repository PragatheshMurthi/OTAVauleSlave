/* Simple simulated HAL implementations for LoRa/WiFi/GSM */
#include "lorahal.h"
#include "comm.h"
#include <stdlib.h>
#include <string.h>

#define LORA_BUFFER_SIZE 256U

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
    static UINT8 au8LoRaReceiveBuffer[LORA_BUFFER_SIZE];
    
    if ( NULL == ppvData ) {
        print_err("receive_LoRa: invalid output pointer\n");
        return ERR_INVALID_PARAM;
    }

    // Simulate receiving an ORDER_BUFFER
    memset(au8LoRaReceiveBuffer, 0, sizeof(au8LoRaReceiveBuffer));
    
    print_info("receive_LoRa: simulated receive\n");
    
    // Return pointer to static buffer
    // Static buffer remains valid after function returns
    *ppvData = au8LoRaReceiveBuffer;
    
    return ERR_OK;
}

