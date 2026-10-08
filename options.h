#ifndef OPTIONS_H
#define OPTIONS_H


#define MAX_ORDER_RESULT_LENGTH 32

/* SX1278 and ESP32 WROOM hardware configuration. */
#define LORA_SCK_PIN 18
#define LORA_MISO_PIN 19
#define LORA_MOSI_PIN 23
#define LORA_SS_PIN 5
#define LORA_RESET_PIN 14
#define LORA_DIO0_PIN 26
#define LORA_SPI_FREQUENCY_HZ 8000000U
#define LORA_FREQUENCY_HZ 433000000L
#define LORA_TX_POWER_DBM 17U
#define LORA_SPREADING_FACTOR 7U
#define LORA_SIGNAL_BANDWIDTH_HZ 125000L
#define LORA_CODING_RATE 5
#define LORA_PREAMBLE_LENGTH 8U
#define LORA_SYNC_WORD 0x34U
#define MAX_ACK_WAIT_TIME 5000U
#define LORA_RX_TIMEOUT_MS MAX_ACK_WAIT_TIME

/* ************** CURRENT VALVE CONFIG **************/

#define MAX_VALVES 4


/* Network Interfaces Configuration */

// LAN Interfaces
//#define ENABLE_LORA_FOR_LAN
#define ENABLE_IPC_SIMULATION
/* Logging configuration */

#define ENABLE_DBG
#define ENABLE_INFO
#define ENABLE_ERROR

#define ENABLE_UART_LOG
//#define ENABLE_NETWORK_LOGG

// Valve to GPIO mapping configuration                  Valve number    GPIO Pin
static char au8AssValves[MAX_VALVES][2] = {     {               0,          1}, 
                                                {               1,          2}, 
                                                {/* Invalid*/  -1,          3}, 
                                                {/* Invalid*/  -1,          4} };
#endif /* OPTIONS_H */

