#include "apiinterface.h"
#include "configurationfilehandler.h"
#include "usbinterface.h"
#include "phasezerohandler.h"
#include "phaseonehandler.h"
#include "phasetwohandler.h"

int debug_level = 0;

apiInterface::apiInterface()
{
    for(int i=0; i<100; i++){
        fprintf(stderr, "WARNING: You just chucked in a meme value of 25000 as the max sample rate.\n");
    }
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw, &num_channels_excluding_ref);
    usbHandler = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    p0handler = new phaseZeroHandler();
    p1handler = new phaseOneHandler(&phase1_raw);
    p2handler = new phaseTwoHandler();
    p3handler = new phaseOneHandler(&phase3_raw);

    //Remove buffering from printf() statements.  Slower, but at least they will be in the right order.
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void apiInterface::testAction(){
    printf("Performing Test Action...\n");
}

void apiInterface::testAction_2(){
    printf("Performing Alternate Test Action...\n");
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

int apiInterface::stopStream(){
    int error;

    error = p3handler->createPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p3handler->createPattern().  Aborting...\n", error);
        return error;
    }
    error = p3handler->sendPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p3handler->sendPattern().  Aborting...\n", error);
        return error+1000;
    }

    p2handler->deleteThread();
    return 0;
}

int apiInterface::startStream(){
    configureSynamps2Device();
    return p2handler->enterPhaseTwo(phase2_length, num_channels_excluding_ref, p1handler->getInterface());
}

std::vector<double>* apiInterface::getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length){
    if(channel > getNumChannelsExcludingRef()){
        fprintf(stderr, "ERROR: Attempted to access nonexistant channel.\n");
        return NULL;
    }
    return p2handler->getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length);
}

int apiInterface::getNumChannelsExcludingRef(){
    return num_channels_excluding_ref;
}
