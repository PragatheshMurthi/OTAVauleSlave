#ifndef NETINTER_H
#define NETINTER_H

/* enums moved to gen.h */
ERROR_CODE initialize_interfaces( GLOBAL_ARCHIVE* pstGlobal );
ERROR_CODE hal_send_to_master( PVOID pvData, UINT16 u16Len );
ERROR_CODE hal_receive_from_master( PVOID *ppvData );
#endif /* NETINTER_H */
