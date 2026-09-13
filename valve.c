/* ========= INCLUDES ========= */
#include "valve.h"

/* ========= FUNCTIONS ========= */

VOID initiate_order( GLOBAL_ARCHIVE* pstGlobalArchive )
{
    ERROR_CODE enErrorCode = ERR_OK;

    DBG_ENTRY

    if ( NULL == pstGlobalArchive )
    {
        set_error(pstGlobalArchive, ERR_INVALID_PARAM);
        print_err("%s:GlobalArchive<KO>", __FUNCTION__);
        DBG_EXIT
        return;
    }

    if ( get_error( pstGlobalArchive ) != ERR_OK )
    {
        print_err("%s:ErrorInPreviousState<KO><%d>", __FUNCTION__, get_error( pstGlobalArchive ));
        DBG_EXIT
        return;
    }


    if ( 0 == pstGlobalArchive->bIsTimerRunning )
    {
        // Initiate the order based on the parsed details in the global archive.
        enErrorCode = hal_actuate_valve( pstGlobalArchive->u8ActionType );

        if ( enErrorCode == ERR_OK )
        {
            pstGlobalArchive->enSlaveState = SLAVE_ORDER_PROCESSED;
            print_dbg("%s:OrderInitiated<OK><VN[%d]AT[%d]TC[%d]SS[%d]>", __FUNCTION__, pstGlobalArchive->u32VaulveNumber, pstGlobalArchive->u8ActionType, pstGlobalArchive->u32TimerCntS, pstGlobalArchive->enSlaveState);
        } else {
            print_err("%s:OrderInitiate<KO>ERR<%d>", __FUNCTION__, enErrorCode);
            set_error(pstGlobalArchive, enErrorCode );
        }

    } else {
        print_dbg("%s:OrderInitiate<KO>TimerRunning<%d>", __FUNCTION__, pstGlobalArchive->bIsTimerRunning);
    }
    
    DBG_EXIT
    return;
}

ERROR_CODE hal_actuate_valve( UINT8 u8ActionType )
{
    // This function should contain the hardware-specific implementation to actuate the valve.
    // For now, we will just simulate the action and return success.
    DBG_ENTRY
    print_dbg("%s:HalActuateValve<Simulated><AT[%d]>", __FUNCTION__, u8ActionType );
    DBG_EXIT
    // Simulate a successful initiation
    return ERR_OK;
}