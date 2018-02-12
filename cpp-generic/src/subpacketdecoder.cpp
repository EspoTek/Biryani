#include "subpacketdecoder.h"
#include "o1buffer.h"

subPacketDecoder::subPacketDecoder(int num_channels_excluding_ref_in)
{
    //Get the right number of channels
    num_channels_including_ref = num_channels_excluding_ref_in + 1;

    //interPacket is a temporary extra subPacket that exists between packet boundaries
    interPacket = (unsigned char*) malloc(numBytesPerSubpacket());

    //Null out all the o1buffer pointers, for debugging purposes
    for(int i=0; i<SUBPACKET_MAX_NUM_CHANNELS; i++){
        sampleBuffer_CH[i] = NULL;
    }

    //Only allocate the o1buffers we need
    for(int i=0; i<num_channels_including_ref; i++){
        sampleBuffer_CH[i] = new o1buffer();
    }
}

int subPacketDecoder::isValidSubPacketStream(unsigned char *ptr_sub, int num_to_test){
    for (int i = 0; i<num_to_test; i++){
        if(!isValidSubPacket(&ptr_sub[i*numBytesPerSubpacket()])){
            //SubPacket failed test!
            return 0;
        }
    }
    //It passed num_to_test tests in a row.  Probably legit.  Return "valid stream".
    return 1;
}

int subPacketDecoder::isValidSubPacket(unsigned char *ptr_sub){
    int subPacketLength = numBytesPerSubpacket();

    if(ptr_sub[subPacketLength-1] != 0x04){
        //The last byte must be 0x04!  Return "not a valid subpacket".
        return 0;
    }

    if(count2int(ptr_sub+subPacketLength) - count2int(ptr_sub) != 1){
        //The next subpacket in the sequence doesn't seem to have a correctly incremented byte.  Return "not a valid subpacket".
        return 0;
    }

    //If it passed those two tests, it's probably OK.
    return 1;
}

int subPacketDecoder::decodeSubPacket(unsigned char *ptr_sub){
    printf_verbose("subPacketDecoder::decodeSubPacket()\n");
    //Match the template to the pointer.
    subPacket_generic_template *temp = (subPacket_generic_template *) ptr_sub;

    //Find the count
    int subPacket_count = count2int(temp->count);

    read_write_mutex.lock();
    //Add all of the channel data
    for (int i=0; i<num_channels_including_ref; i++){
        sampleBuffer_CH[i]->add(temp->channelData[i], subPacket_count);
    }
    read_write_mutex.unlock();
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

int subPacketDecoder::count2int(unsigned char *ptr_count){
    //first byte is lowest 8 bits, third byte is highest.
    return ptr_count[0] + 256 * ptr_count[1] + 65536 * ptr_count[2];
}
