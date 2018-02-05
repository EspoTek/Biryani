#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <vector>
#include <iostream>
#include "api_defines.h"

class configurationFileHandler;
class phaseZeroHandler;
class usbInterface;

//apiInterface is the main class of the Biryani 7 API.
//It takes all external calls and routes them to the appropriate objects.

class apiInterface
{
public:
    apiInterface();
    void testAction();
    int loadFile(char *fname);
    int initialiseSynamps2Device();
private:
    //Internal object pointers
    configurationFileHandler *fileHandler;
    phaseZeroHandler *p0handler;
    usbInterface *usbHandler;

    //"Global" (within API) variables
    std::vector<rawPacket> phase1_raw;
    int phase2_length;
    std::vector<rawPacket> phase3_raw;
};

#endif // APIINTERFACE_H
