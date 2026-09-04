#ifndef OPTIONS_H
#define OPTIONS_H


#define MAX_ORDER_RESULT_LENGTH 32

###############################################################
##                                                           ##
##                    CURRENT VALVE CONFIG                   ##
##                                                           ##
###############################################################

#define SELF_VALVE_NUMBER 0


###############################################################
##                                                           ##
##          Network Interfaces Configuration                 ##
##                                                           ##
###############################################################

// LAN Interfaces
#define ENABLE_LORA_FOR_LAN

// WAN Interfaces
#define ENABLE_WIFI_FOR_WAN
//#define ENABLE_GSM_FOR_WAN

###############################################################
##                                                           ##
##          Logging configuration                            ##
##                                                           ##
###############################################################

#define ENABLE_DBG
#define ENABLE_INFO
#define ENABLE_ERROR

#define ENABLE_UART_LOG
//#define ENABLE_NETWORK_LOGG

#endif /* OPTIONS_H */
