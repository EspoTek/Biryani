#include "apiinterface.h"
#include "configurationfilehandler.h"
#include "usbinterface.h"
#include "phasezerohandler.h"
#include "phaseonehandler.h"
#include "phasetwohandler.h"

int debug_level = 0;

apiInterface::apiInterface()
{
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw);
    usbHandler = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    p0handler = new phaseZeroHandler();
    p1handler = new phaseOneHandler(&phase1_raw);
    p2handler = new phaseTwoHandler();

    //Remove buffering from printf() statements.  Slower, but at least they will be in the right order.
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void apiInterface::testAction(){
    printf("Performing Test Action...\n");
    p2handler->enterPhaseTwo(phase2_length, p1handler->getInterface());
}

void apiInterface::testAction_2(){
    printf("Performing Alternate Test Action...\n");
    p2handler->deleteThread();
}


void apiInterface::setDebugLevel(int new_debug_level_in){
    debug_level = new_debug_level_in;
    printf("Debug Level changed to %d\n", debug_level);
}

int apiInterface::loadFile(char *fname){
    return fileHandler->loadFile(fname);
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
            error = p0handler->createPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->createInitPattern(): 0x%08x", error);
                return error;
            }
            error = p0handler->sendPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->sendInitPattern(): 0x%08x", error);
                return error;
            }
            return 0;
        case 1:
            printf("Synamps2 device already connected and ready.  Returning without initialising.\n");
            return 420;
        default:
            fprintf(stderr, "ERROR in initialiseSynamps2Device(), amplifer status invalid", error);
            return -69;
    }
}

int apiInterface::configureSynamps2Device(){
    int error;

    error = p1handler->createPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p1handler->createPattern().  Aborting...\n", error);
        return error;
    }
    error = p1handler->sendPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p1handler->sendPattern().  Aborting...\n", error);
        return error+1000;
    }
    return 0;
}

