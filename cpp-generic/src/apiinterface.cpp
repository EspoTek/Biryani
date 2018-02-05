#include "apiinterface.h"
#include "configurationfilehandler.h"
#include "usbinterface.h"
#include "phasezerohandler.h"

int debug_level = 0;

apiInterface::apiInterface()
{
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw);
    usbHandler = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    p0handler = new phaseZeroHandler();
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

int apiInterface::loadFile(char *fname){
    return fileHandler->loadFile(fname);
}

void apiInterface::testAction(){
    printf("Performing Test Action...\n");
}


int apiInterface::initialiseSynamps2Device(){
    int error = 0;

    int amplifierStatus = p0handler->checkIfAlreadyInitialised();

    switch(amplifierStatus){
        case -1:
            fprintf(stderr, "Synamps2 device not detected!\n");
            return -420;
        case 0:
            printf("Synamps2 device connected but not ready.  Connecting...\n");
            error = p0handler->createInitPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->createInitPattern(): 0x%08x", error);
                return error;
            }
            error = p0handler->sendInitPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->sendInitPattern(): 0x%08x", error);
                return error;
            }
            return 0;
        case 1:
            printf("Synamps2 device connected and ready.  Returning.\n");
            return 420;
        default:
            fprintf(stderr, "ERROR in initialiseSynamps2Device(), amplifer status invalid", error);
    }
}
