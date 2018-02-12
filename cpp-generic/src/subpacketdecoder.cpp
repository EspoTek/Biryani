#include "subpacketdecoder.h"

subPacketDecoder::subPacketDecoder(int num_channels_excluding_ref_in)
{
    //Set all of the subPacket samples to 0, for better debugging.
    for (int i=0; i<SUBPACKET_MAX_NUM_CHANNELS; i++){
        subPacket_samples[i] = 0;
    }

    //Get the right number of channels
    num_channels_including_ref = num_channels_excluding_ref_in + 1;

    //interPacket is a temporary extra subPacket that exists between packet boundaries
    interPacket = (unsigned char*) malloc(numBytesPerSubpacket());
}

int subPacketDecoder::isValidSubPacketStream(unsigned char *ptr_sub, int num_to_test){
    return 0;
}

int subPacketDecoder::isValidSubPacket(unsigned char *ptr_sub){
    return 0;
}

int subPacketDecoder::decodeSubPacket(unsigned char *ptr_sub){
    return 0;
}

int subPacketDecoder::decodeInterPacket(){
    return decodeSubPacket(interPacket);
}

int subPacketDecoder::numBytesPerSubpacket(){
    return num_channels_including_ref * 4 + 4;  //4 bytes per sample, plus 3 for the count, plus 1 for the final byte
}

int subPacketDecoder::fill_interPacket_back(unsigned char *ptr, int packetStartOffset){
    if(packetStartOffset==0){
        return 1;
    }
    //Fill the back
    memcpy(&interPacket[numBytesPerSubpacket() - packetStartOffset], ptr, packetStartOffset);
    return 0;
}


int subPacketDecoder::fill_interPacket_front(unsigned char *ptr, int numLeftoverBytes){
    memcpy(interPacket, ptr, numLeftoverBytes);
    return 0;
}
