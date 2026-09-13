#ifndef IPC_H
#define IPC_H

#include "gen.h"

/* Minimal IPC simulation backend interface */
ERROR_CODE initialize_ipc( void );
ERROR_CODE send_ipc( PVOID pvData, UINT16 u16Len );
ERROR_CODE receive_ipc( PVOID *ppvData );

#endif /* IPC_H */
