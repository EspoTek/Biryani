#ifndef SUBPACKETDECODER_H
#define SUBPACKETDECODER_H

#include "api_defines.h"
#include <mutex>

class o1buffer;


#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
    #error Structure of subPacket_generic_template assumes a little-endian CPU architecture.  You will need to rewrite it for whatever architecture you're using now.
#endif


//Definition of SubPacket template structure.  See "Synamps2 Spec" document on Google Drive.
//https://docs.google.com/document/d/12KfWQ2ZpVhNfjOQ_vc9-9VN2BYKmxVJRjkxDZdsD5Fc/edit?usp=sharing
#pragma pack(push, 1)  //Force the struct to be unpacked, otherwise it will assume nonexistant padding bytes.
typedef struct subPacket_generic_template{
    unsigned char count[3];
    int channelData[SUBPACKET_MAX_NUM_CHANNELS];
} subPacket_generic_template;
#pragma pack(pop)

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
    std::vector<double>* getDownSampledChannelData_double(int channel, double sampleRate_hz, int mode, double delay_seconds, double timeWindow_seconds, int* length);
private:
    unsigned char * interPacket = NULL;
    int num_channels_including_ref;
    o1buffer *sampleBuffer_CH[SUBPACKET_MAX_NUM_CHANNELS];
    int isValidSubPacket(unsigned char* ptr_sub);
    int count2int(unsigned char *ptr_count);
    std::mutex read_write_mutex;
};

#endif // SUBPACKETDECODER_H
