#ifndef USERDEFINEDPROPERTIES_H
#define USERDEFINEDPROPERTIES_H

#define NUM_DATA_CHANNELS 5
#define LENGTH_DATA_PACKET 4096
#define EXPECTED_FINAL_BYTE 0x04

//For plotting
#define MAX_SAMPLES_PER_SECOND 25000
#define MAX_CONVERTED_SAMPLES_STORED 4096


//Definition of SubPacket structure.  See "Synamps2 Spec" document on Google Drive.
//https://docs.google.com/document/d/12KfWQ2ZpVhNfjOQ_vc9-9VN2BYKmxVJRjkxDZdsD5Fc/edit?usp=sharing
#pragma pack(push, 1)  //Force the struct to be unpacked, otherwise it will assume nonexistant padding bytes.
typedef struct subPacket{
    unsigned char count[3];
    int channelData[NUM_DATA_CHANNELS];
    unsigned char finalByte;
} subPacket;
#pragma pack(pop)



#endif // USERDEFINEDPROPERTIES_H
