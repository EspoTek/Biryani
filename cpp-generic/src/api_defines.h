#ifndef API_DEFINES_H
#define API_DEFINES_H

#define SYNAMPS2_MAIN_VID 0x0B6E
#define SYNAMPS2_MAIN_PID 0x0006

#define SYNAMPS2_UNINITIALISED_VID 0x0B6E
#define SYNAMPS2_UNINITIALISED_PID 0x0005

typedef struct rawPacket{
    unsigned char *dataPointer;
    int length;
}rawPacket;

#endif // API_DEFINES_H
