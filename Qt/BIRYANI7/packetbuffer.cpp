#include "packetbuffer.h"
#include "o1buffer.h"
#include <QDebug>

//These are defined up here basically just to prevent multiply defined symbols.
//They should go back to userdefinedproperties.h when I fix up the headers-in-headers.

//Count 2 integer
int c2i(unsigned char* count){
    //first byte is lowest 8 bits, third byte is highest.
    return count[0] + 256*count[1] + 65536*count[2];
}

//This function takes a sequence of subpackets (supposedly), and checks to see whether it could actually be a valid sequence
bool isSubPacketSequence(subPacket *spArray, int num2check){
    for(int i=1;i<num2check;i++){
        //Valid subpackets should have an incrementing count field between each subpacket
        int count_a = c2i(spArray[i-1].count);
        int count_b = c2i(spArray[i].count);
        if((count_b - count_a) != 1){
            qDebug("Inter-packet difference of %d, i=%d", (c2i((spArray[i].count))-c2i((spArray[i-1].count))), i);
            return false;
        }
        //The final byte should be unchanged.
        if(spArray[i].finalByte != EXPECTED_FINAL_BYTE){
            qDebug("Final Byte is not 0x%02x, i=%d", EXPECTED_FINAL_BYTE, i);
            return false;
        }
    }
    //No problems found
    return true;
}

//PacketBuffer is a class for temporarily storing whole packets.
//It can store NUM_PACKETS_KEPT past packets, and automatically handles the pointers.
//It also decodes the packets and stores them in in buffers
packetBuffer::packetBuffer(QObject *parent) : QObject(parent)
{
    //numLeftoverBytes = LENGTH_DATA_PACKET%sizeof(subPacket);
    qDebug() << "subPacket is of size" << sizeof(subPacket);

    interPacketBuffer = (unsigned char *) malloc(sizeof(subPacket));
    tempSubPacket = (subPacket *) interPacketBuffer;

    for (int i=0;i<NUM_DATA_CHANNELS;i++){
        buffer_CH[i] = new o1buffer();
    }
}

//This function adds a packet to the buffer.
void packetBuffer::addPacket(unsigned char* packetBuffer){
    memcpy(&storageBuffer[0][nextPacketIndex],packetBuffer,LENGTH_DATA_PACKET);
    nextPacketIndex++;
    if(nextPacketIndex>MAX_PACKET_INDEX){
        nextPacketIndex = 0;
    }
}

//This function gets the (n+1)th last packet that was added.
//For example, getPacket(0) would return the last packet, and getPacket(1) would return the packet before that.
unsigned char* packetBuffer::getPacket(unsigned int position){
    if(position>NUM_PACKETS_KEPT){
        qFatal("packetBuffer::getPacket can only store %d elements, but is being asked to provide the %dth", NUM_PACKETS_KEPT, position);
    }
    int tempIndex = nextPacketIndex-position-1;
    if(tempIndex<0){
        tempIndex += NUM_PACKETS_KEPT;
    }

    return &storageBuffer[tempIndex][0];
}

//This function extracts the necessary subpackets from the packets and places them in the o1buffer internally
void packetBuffer::decodePacket(){
    qDebug() << "packetBuffer::decodePacket()";
    unsigned char* beginPacketPointer = (unsigned char *) &storageBuffer[0][lastPacketIndex()];
    //Do not start decoding at the start of the packet.  The first byte could be halfway through a subpacket!
    subPacket *subPacketPointer = (subPacket*) (beginPacketPointer + packetStartOffset);

    if(!startingPacketFound){
        //It's highly probably that we've started in a random position mid-subpacket.
        //This code should find where, specifically.
        for(int i=0; i<sizeof(subPacket); i++){
            if(isSubPacketSequence(subPacketPointer, 32)){  //32 things in a row that look like subpackets should indicate a subpacket stream.  If it looks like a duck, and quacks like a duck...
                startingPacketFound = true;
                break;
            }
            packetStartOffset++;
            subPacket *subPacketPointer = (subPacket*) (beginPacketPointer + packetStartOffset);
        }
    }

    //subPacketPointer should be pointing to the first whole subpacket in the stream.
    //But is it?
    if(!isSubPacketSequence(subPacketPointer, 32)){
        startingPacketFound = false;
        qFatal("subPacketPointer, element of packetBuffer::decodePacket, is not pointing to a subPacket!");
    }
    //But what about the partial subpacket that could be before it?
    if(packetStartOffset!=0){
        //Fill the back part of the interPacketBuffer.  It should now contain a whole subpacket.
        memcpy(&interPacketBuffer[sizeof(subPacket) - packetStartOffset], beginPacketPointer, packetStartOffset);
        for (int i=0;i<NUM_DATA_CHANNELS;i++){
            buffer_CH[i]->add(tempSubPacket[0].channelData[i], c2i(tempSubPacket[0].count));
        }
    }
    //But how many packets follow subPacketPointer?
    int numPacketsToDecode = (LENGTH_DATA_PACKET-packetStartOffset)/sizeof(subPacket);
    int numLeftoverBytes = (LENGTH_DATA_PACKET-packetStartOffset)%sizeof(subPacket);

    //Now we actually decode the packets.
    int sequenceNumber;
    for (int i=0;i<numPacketsToDecode;i++){
        sequenceNumber = c2i(subPacketPointer[i].count);
        for (int j=0;j<NUM_DATA_CHANNELS;j++){
            buffer_CH[j]->add(subPacketPointer[i].channelData[j], sequenceNumber);
        }
    }

    //But there's that leftover data at the end.
    memcpy(interPacketBuffer, &beginPacketPointer[LENGTH_DATA_PACKET-numLeftoverBytes], numLeftoverBytes);

    //And don't forget to update the packet start offset for the next packet!
    packetStartOffset = sizeof(subPacket) - numLeftoverBytes;
}

bool packetBuffer::ready(){
    return startingPacketFound;
}

unsigned int packetBuffer::lastPacketIndex(){
    int temp = nextPacketIndex-1;
    if(temp<0){
        temp += NUM_PACKETS_KEPT;
    }
    return temp;
}

QVector<double>** packetBuffer::getDownSampledChannelData(double sampleRate, int mode){
    qDebug() << "Address is" << buffer_CH[0]->mostRecentAddress;
    return NULL;
}
