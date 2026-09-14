/* Headers */
#include "gen.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "netinter.h"
#include "comm.h"
#include "valve.h"

/* Prototypes */

// Main routine
void startWorking (void);
void cleanup (void);

/* Global variables */
static GLOBAL_ARCHIVE *gstInformationDB;

/* Defenitions */
void setup() {
  ERROR_CODE enErrorCode = ERR_OK;

  // Create memory for the global information data base.
  gstInformationDB = malloc ( sizeof ( GLOBAL_ARCHIVE ));
  if ( NULL == gstInformationDB ) {
    print_err("setup:MemoryAllocationFailed<KO>SIZE<%zu>", sizeof(GLOBAL_ARCHIVE));
    return;
  }
  
  memset( gstInformationDB, 0, sizeof(GLOBAL_ARCHIVE));

  // Initialize the network modules for com.
  enErrorCode = initialize_interfaces( gstInformationDB );
  if ( enErrorCode != ERR_OK) {
      // Handle the error here
      print_err("%s:NetModInit<KO>ERR<%d>",__FUNCTION__,enErrorCode);
      free( gstInformationDB );
      gstInformationDB = NULL;
      return;
  }

  gstInformationDB->enSlaveState = SLAVE_READY;
}

void loop() {

  // All the core listening functionalities...
  startWorking();
  print_dbg("Work halted...");
  cleanup();

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

}

void cleanup (void) {
  if ( NULL != gstInformationDB ) {
    // Free the global archive database
    free( gstInformationDB );
    gstInformationDB = NULL;
    print_dbg("Global Archive DB cleaned up");
  }
}

int main(int argc, char **argv)
{
  (void)argc;(void)argv;
  setup();
  loop();
  return 0;
}