#include "apiinterface.h"
#include "configurationfilehandler.h"

apiInterface::apiInterface()
{
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw);
}

int apiInterface::loadFile(char *fname){
    return fileHandler->loadFile(fname);
}
