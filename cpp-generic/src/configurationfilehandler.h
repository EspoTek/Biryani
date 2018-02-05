#ifndef CONFIGURATIONFILEHANDLER_H
#define CONFIGURATIONFILEHANDLER_H

#include <vector>
#include <iostream>
#include "api_defines.h"

//This class handles the reading and parsing of .bcf files.
//No actual processing of data is handled here.

class configurationFileHandler
{
public:
    configurationFileHandler(std::vector<rawPacket> *ptr_phase1_raw_in, int *ptr_phase2_length_in, std::vector<rawPacket> *ptr_phase3_raw_in);
    int loadFile(char *fname);
private:
    std::vector<rawPacket> *ptr_phase1_raw;
    int *ptr_phase2_length;
    std::vector<rawPacket> *ptr_phase3_raw;
    int fill_raw_packet_structure(rawPacket* packet, char *inputString, int phase);

};

#endif // CONFIGURATIONFILEHANDLER_H
