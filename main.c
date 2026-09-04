/* Headders */
#include "gen.h"

/* Prototypes */

// Main routine
void startWorking (void);

/* Global variables */
static GLOBAL_ARCHIVE *gstInformationDB;

/* Defenitions */
void setup() {
  ERROR_CODE enErrorCode = ERR_OK;

  // Create memory for the global information data base.
  gstInformationDB = malloc ( sizeof ( GLOBAL_ARCHIVE ));
  if ( NULL != gstInformationDB )
    memset( gstInformationDB, 0, sizeof(GLOBAL_ARCHIVE));

  // Initialize the network modules for com.
  enErrorCode = initializeInterfaces( gstInformationDB );
  if ( enErrorCode != ERR_OK) {
      // Handle the error here
      print_err("%s:NetModInit<KO>ERR<%d>",__FUNCTION__,enErrorCode);
      return;
  }

  gstInformationDB ? gstInformationDB->enSlaveState = SLAVE_READY : NULL;
}

void loop() {

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
    pvOrderBuffer = expect_order ( &gstInformationDB );
    parse_order( &gstInformationDB, pvOrderBuffer );
    initiate_order( &gstInformationDB );
    update_master( &gstInformationDB );

  }

}