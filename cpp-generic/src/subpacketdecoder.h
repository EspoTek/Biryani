#ifndef SUBPACKETDECODER_H
#define SUBPACKETDECODER_H

#include "api_defines.h"

class subPacketDecoder
{
public:
    subPacketDecoder(int num_channels_excluding_ref_in);
    int isValidSubPacketStream(unsigned char *ptr_sub, int num_to_test);
    int decodeSubPacket(unsigned char *ptr_sub);
    int numBytesPerSubpacket();
    int fill_interPacket_back(unsigned char *ptr, int packetStartOffset);
    int fill_interPacket_front(unsigned char *ptr, int numLeftoverBytes);
    int decodeInterPacket();
private:
    unsigned char * interPacket = NULL;
    int subPacket_count = 0;
    int num_channels_including_ref;
    int subPacket_samples[SUBPACKET_MAX_NUM_CHANNELS];
    int isValidSubPacket(unsigned char* ptr_sub);
};

#endif // SUBPACKETDECODER_H
