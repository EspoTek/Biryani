#ifndef API_DEFINES_H
#define API_DEFINES_H

#define FILE_MAX_CHARS_PER_LINE 300000
#define FILE_BCF_HEADER_STRING "Biryani 7 Configuration File Version 0.91\n"
#define USB_BULK_TIMEOUT_MS 500  //Timeout for USB bulk transfers in ms.
#define SUBPACKET_STREAM_START_INDEX 0x08 //This is the first packet that subPacketDecoder will look for when starting the stream decoding.

#define SYNAMPS2_MAIN_VID 0x0B6E
#define SYNAMPS2_MAIN_PID 0x0006

#define SYNAMPS2_UNINITIALISED_VID 0x0B6E
#define SYNAMPS2_UNINITIALISED_PID 0x0005

#define SUBPACKET_MAX_NUM_CHANNELS 69 //Hehe

#define NUM_SAMPLES_PER_CHANNEL 16777215 //AKA buffer size, in words.
//16777215 = 2^24 - 1.  Highest number that can be stored in the 24 bit address.

#define MAX_SAMPLES_PER_SECOND 20000 //Samples per second that the Synamps2 device records at.  From the spec sheet.  http://compumedicsneuroscan.com/wp-content/uploads/2014/03/AE074_4-SynAmps-RT-specs-sheet-USA-LR.pdf

//Global includes
#include <vector>
#include <iostream>
#include <stdio.h>


//rawPacket struct definition
typedef struct rawPacket{
    unsigned char *dataPointer;
    int length;
}rawPacket;


//Debugging
#define DEBUG_LEVEL_VERBOSE 400
#define DEBUG_LEVEL_DEBUGGING 300
extern int debug_level;  //Should not be changed after calling a function

#define printf_verbose(...)\
    if(debug_level >= DEBUG_LEVEL_VERBOSE){ \
        printf(__VA_ARGS__);\
    } \

#define printf_debugging(...)\
    if(debug_level >= DEBUG_LEVEL_DEBUGGING){ \
        printf(__VA_ARGS__);\
    } \







#endif // API_DEFINES_H
