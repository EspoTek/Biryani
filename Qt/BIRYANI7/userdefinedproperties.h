#ifndef USERDEFINEDPROPERTIES_H
#define USERDEFINEDPROPERTIES_H

#define NUM_DATA_CHANNELS 4
#define LENGTH_DATA_PACKET 4096
#define EXPECTED_FINAL_BYTE 0x04

//Definition of SubPacket structure.  See "Synamps2 Spec" document on Google Drive.
//https://docs.google.com/document/d/12KfWQ2ZpVhNfjOQ_vc9-9VN2BYKmxVJRjkxDZdsD5Fc/edit?usp=sharing
typedef struct subPacket{
    unsigned char count[3];
    int channelData[NUM_DATA_CHANNELS];
    unsigned char finalByte;
} subPacket;


#endif // USERDEFINEDPROPERTIES_H
