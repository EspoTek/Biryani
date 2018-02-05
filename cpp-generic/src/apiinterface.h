#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <vector>
#include <iostream>
#include "rawpacket.h"

class configurationFileHandler;

class apiInterface
{
public:
    apiInterface();
    int loadFile(char *fname);
private:
    std::vector<rawPacket> phase1_raw;
    int phase2_length;
    std::vector<rawPacket> phase3_raw;
    configurationFileHandler *fileHandler;
};

#endif // APIINTERFACE_H
