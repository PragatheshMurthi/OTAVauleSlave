/*******************************************************************************
 * @file        main.c
 * @brief       Main entry point and core event loop for the OTA Valve Slave.
 * @author      Pragathesh Murthi <pragathesh.murthi@example.com>
 * @date        2024-04-20
 * 
 * @license     MIT License
 *              Copyright (c) 2024 Valve Control System
 *              All rights reserved.
 ******************************************************************************/

/*============================================================================*/
/*                                  INCLUDES                                  */
/*============================================================================*/
#include "gen.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "netinter.h"
#include "comm.h"
#include "valve.h"

/*============================================================================*/
/*                         DEFINES & MACROS & ENUMS                           */
/*============================================================================*/
/* None */

/*============================================================================*/
/*                            FUNCTION PROTOTYPES                             */
/*============================================================================*/
void startWorking(void);
void cleanup(void);

/*============================================================================*/
/*                          GLOBAL / STATIC VARIABLES                         */
/*============================================================================*/
static GLOBAL_ARCHIVE *gstInformationDB;

/*============================================================================*/
/*                           FUNCTION DEFINITIONS                             */
/*============================================================================*/
static GLOBAL_ARCHIVE *gstInformationDB;

/* Defenitions */
void vmain_setup() {
  ERROR_CODE enErrorCode = ERR_OK_INTR;

  // Create memory for the global information data base.
  gstInformationDB = malloc ( sizeof ( GLOBAL_ARCHIVE ));
  if ( NULL == gstInformationDB ) {
    print_err("setup:MemoryAllocationFailed<KO>SIZE<%zu>", sizeof(GLOBAL_ARCHIVE));
    return;
  }
  
  memset( gstInformationDB, 0, sizeof(GLOBAL_ARCHIVE));

  // Initialize the network modules for com.
  enErrorCode = initialize_interfaces( gstInformationDB );
  if ( enErrorCode != ERR_OK_INTR) {
      // Handle the error here
      print_err("%s:NetModInit<KO>ERR<%d>",__FUNCTION__,enErrorCode);
      free( gstInformationDB );
      gstInformationDB = NULL;
      return;
  }

  gstInformationDB->enSlaveState = SLAVE_READY;
}

void vmain_loop() {
  // All the core listening functionalities...
  startWorking();
  print_dbg("Work halted...");
}

void startWorking (void) {

  if (gstInformationDB == NULL) {
    print_err("Global Archive DB not initialized/NULL");
    return;
  }
  
  while ( gstInformationDB->enSlaveState > SLAVE_STOPPED ) {
    
    PVOID pvOrderBuffer = NULL;
    // Expect Order from the master
    pvOrderBuffer = expect_order ( gstInformationDB );
    parse_order( gstInformationDB, pvOrderBuffer );
    initiate_order( gstInformationDB );
    update_master( gstInformationDB );

  }
  cleanup();

}

void cleanup (void) {
  if ( NULL != gstInformationDB ) {
    // Free the global archive database
    free( gstInformationDB );
    gstInformationDB = NULL;
    print_dbg("Global Archive DB cleaned up");
  }
}

#ifdef ENABLE_IPC_SIMULATION
int main(int argc, char **argv)
{
  (void)argc;(void)argv;
  setup();
  loop();
  return 0;
}
#endif