#ifndef PHASEZEROHANDLER_H
#define PHASEZEROHANDLER_H

#include "api_defines.h"

class initTransfer;
class usbInterface;

class phaseZeroHandler
{
public:
    phaseZeroHandler();
    ~phaseZeroHandler();
    int checkIfAlreadyInitialised();
    int createPattern(void);
    int sendPattern(void);
private:
    std::vector<initTransfer*> initPattern;
    usbInterface * phase0_interface = NULL;
};

#endif // PHASEZEROHANDLER_H
