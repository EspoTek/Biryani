#ifndef API_DEFINES_H
#define API_DEFINES_H

#define FILE_MAX_CHARS_PER_LINE 65535
#define FILE_BCF_HEADER_STRING "Biryani 7 Configuration File Version 0.91\n"


#define SYNAMPS2_MAIN_VID 0x0B6E
#define SYNAMPS2_MAIN_PID 0x0006

#define SYNAMPS2_UNINITIALISED_VID 0x0B6E
#define SYNAMPS2_UNINITIALISED_PID 0x0005

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
