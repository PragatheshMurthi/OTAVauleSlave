#ifndef OPTIONS_H
#define OPTIONS_H


#define MAX_ORDER_RESULT_LENGTH 32

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

