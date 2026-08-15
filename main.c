
#include "gen.h"

static GLOBAL_ARCHIVE *gstInformationDB;

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
}

void loop() {
  // put your main code here, to run repeatedly:

}
