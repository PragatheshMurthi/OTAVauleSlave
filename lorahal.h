#ifndef LORAHAL_H
#define LORAHAL_H

#include "gen.h"

/* Minimal HAL interface for simulation */
ERROR_CODE initialize_LoRa( void );

ERROR_CODE send_LoRa( PVOID pvData, UINT16 u16Len );
ERROR_CODE receive_LoRa( PVOID *ppvData );

#endif /* LORAHAL_H */