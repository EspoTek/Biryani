#include "configurationfilehandler.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "global_constants.h"

#define fgets_check_errors(); \
if(fgets_returned != tempString){ \
    fprintf(stderr, "ERROR: fgets failed at line %d\n", current_line); \
    return 2; \
} else current_line++; \


configurationFileHandler::configurationFileHandler(std::vector<rawPacket> *ptr_phase1_raw_in, int *ptr_phase2_length_in, std::vector<rawPacket> *ptr_phase3_raw_in)
{
    ptr_phase1_raw = ptr_phase1_raw_in;
    ptr_phase2_length = ptr_phase2_length_in;
    ptr_phase3_raw = ptr_phase3_raw_in;
}

int configurationFileHandler::loadFile(char *fname){
    //Debug info.
    printf_verbose("\nconfigurationFileHandler::loadFile(%.1024s)\n", fname);
      ////////////////////////////////
     //////////Open the File/////////
    ////////////////////////////////
    FILE* fptr;
    fptr = fopen(fname, "r");
    if(fptr == NULL){
        fprintf(stderr, "Error: file %s could not be opened\n", fname);
        return 1;
    }

      ////////////////////////////////
     ////////Read the Header/////////
    ////////////////////////////////
    int current_line = 1;
    //tempString stores each line temporarily before it's parsed
    char *tempString = (char *)calloc(FILE_MAX_CHARS_PER_LINE + 1, sizeof(char));
    char * fgets_returned = fgets(tempString, FILE_MAX_CHARS_PER_LINE, fptr);
    fgets_check_errors();

    //Check that the header matches what is expected
    if(strcmp(tempString, FILE_BCF_HEADER_STRING)){
        fprintf(stderr, "ERROR: Header Incorrect.\nExpecting ""%s""\nRead ""%s""\n", FILE_BCF_HEADER_STRING, tempString);
        return 2;
    } else printf_verbose("%s", tempString);

      /////////////////////////////////////////////
     ////////Read the NUM_LINES Variable /////////
    /////////////////////////////////////////////
    //Current position: line 2
    fgets_returned = fgets(tempString, FILE_MAX_CHARS_PER_LINE, fptr);
    fgets_check_errors();

    //Check for the NUM_LINES text
    if(strncmp(tempString, "NUM_LINES", 9)){
        fprintf(stderr, "ERROR: NUM_LINES variable not found!\n");
        return 3;
    }
    //Read in number of lines to parse
    int num_lines_to_parse;
    sscanf(tempString, "NUM_LINES %d", &num_lines_to_parse);
    printf_verbose("Lines to parse: %d\n", num_lines_to_parse);

      /////////////////////////////////////
     ////////Extract the Packets /////////
    /////////////////////////////////////
    int phase_1_counter = 0;
    int phase_3_counter = 0;
    rawPacket rawPacket_temp;

    for (int i=0;i<num_lines_to_parse;i++){
        //Read the line to a string
        fgets_returned = fgets(tempString, FILE_MAX_CHARS_PER_LINE, fptr);
        fgets_check_errors();

        //Check packet type
        if(!strncmp(tempString, "PHASE_1_CONFIG", 14)){
            //Phase 1 packet
            printf_verbose("Phase 1 Packet!\n");
            fill_raw_packet_structure(&rawPacket_temp, tempString, 1);
            ptr_phase1_raw->push_back(rawPacket_temp);
        } else if(!strncmp(tempString, "PHASE_2_DATA", 12)) {
            printf_verbose("Phase 2 Packet!\n");
            sscanf(tempString, "PHASE_2_DATA %d", ptr_phase2_length);
        } else if(!strncmp(tempString, "PHASE_3_CLEANUP", 15)) {
            printf_verbose("Phase 3 Packet!\n");
            fill_raw_packet_structure(&rawPacket_temp, tempString, 3);
            ptr_phase3_raw->push_back(rawPacket_temp);
        } else {
            fprintf(stderr, "Error in configurationFileHandler::loadFile(); Invalid Phase!\n");
            return 4;
        }
    }

    printf_verbose("All packets read!\n");


      /////////////////////////////////
     //////////Close the File/////////
    /////////////////////////////////
    fclose(fptr);

    printf_verbose("File closed!\n");
    return 0;
}

int configurationFileHandler::fill_raw_packet_structure(rawPacket* packet, char* inputString, int phase){
    //Copy the string, so we don't destroy the original
    char tempString[FILE_MAX_CHARS_PER_LINE];
    strcpy(tempString, inputString);
      /////////////////////////////////////////////////
     //////////Get the Length of the Packet///////////
    /////////////////////////////////////////////////
    switch(phase){
        case 1:
            sscanf(tempString, "PHASE_1_CONFIG %d", &packet->length);
            printf_verbose("packet->length = %d\n", packet->length);
            break;
        case 3:
            sscanf(tempString, "PHASE_3_CLEANUP %d", &packet->length);
            printf_verbose("packet->length = %d\n", packet->length);
            break;
        default:
            fprintf(stderr, "ERROR in configurationFileHandler::fill_raw_packet_structure(); invalid phase %d\n", phase);
            return 1;
    }

      /////////////////////////////////////////////////
     //////////Fill the Packet's Data Field///////////
    /////////////////////////////////////////////////
    //Internal variables
    char *internalSubstring;
    int burn = 0;

    //Allocate space for the data field
    packet->dataPointer = (unsigned char *)malloc(packet->length);
    internalSubstring = strtok(tempString, " ");  //Read to first space
    for(int i = 0; i<packet->length + 2;i++){
        printf_verbose("%s ", internalSubstring);
        //Burn the first two substrings; they're an identifier and a length and don't represent data
        if(burn >= 2){
            sscanf(internalSubstring, "%hhx", &packet->dataPointer[i-2]);  //i-2 because the first two substrings are "burned"
        } else burn++;
        internalSubstring = strtok (NULL, " ");
    }
    printf_verbose("\n");

    return 0;
}
