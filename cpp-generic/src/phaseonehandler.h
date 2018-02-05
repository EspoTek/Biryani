#ifndef PHASEONEHANDLER_H
#define PHASEONEHANDLER_H

#include "api_defines.h"

class initTransfer;
class usbInterface;

class phaseOneHandler
{
public:
    phaseOneHandler(std::vector<rawPacket> *phase1_raw_in);
    int createPattern();
private:
    std::vector<initTransfer*> configPattern;
    std::vector<rawPacket> *phase1_raw;
    usbInterface * phase1_interface;
};

#endif // PHASEONEHANDLER_H
