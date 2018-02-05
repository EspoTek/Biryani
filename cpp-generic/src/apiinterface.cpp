#include "apiinterface.h"
#include "configurationfilehandler.h"
#include "usbinterface.h"
#include "phasezerohandler.h"

apiInterface::apiInterface()
{
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw);
    usbHandler = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
}

int apiInterface::loadFile(char *fname){
    return fileHandler->loadFile(fname);
}

void apiInterface::testAction(){
    p0handler = new phaseZeroHandler();
    p0handler->checkIfAlreadyInitialised();
    p0handler->createInitPattern();
    p0handler->sendInitPattern();
}
