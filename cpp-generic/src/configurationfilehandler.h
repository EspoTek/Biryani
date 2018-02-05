#ifndef CONFIGURATIONFILEHANDLER_H
#define CONFIGURATIONFILEHANDLER_H

#include <vector>
#include <iostream>
#include "rawpacket.h"

class configurationFileHandler
{
public:
    configurationFileHandler();
    int loadFile(char *fname);
private:
    std::vector<rawPacket> phase1_raw;
    int phase2_length;
    std::vector<rawPacket> phase3_raw;
    int fill_raw_packet_structure(rawPacket* packet, char *inputString, int phase);

};

#endif // CONFIGURATIONFILEHANDLER_H
