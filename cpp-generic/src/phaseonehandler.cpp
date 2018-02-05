#include "phaseonehandler.h"
#include "inittransfer.h"

phaseOneHandler::phaseOneHandler(std::vector<rawPacket> *phase1_raw_in)
{
    phase1_raw = phase1_raw_in;
    phase1_interface = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    if(phase1_interface == NULL){
        fprintf(stderr, "FATAL ERROR: phase1_interface is NULL\n");
    }
}


//This function creates a chain of packets that, when sent one after another, will configure a (ready) Synamps2 device.
int phaseOneHandler::createPattern(void){
    //Check if a file has been loaded
    if(!phase1_raw->size()){
        fprintf(stderr, "\nERROR: No phase 1 packets detected.  Configuration cannot be performed.  It's possible that no .bcf file has been loaded, or the file that was loaded does not contain the correct data.\nIn the case of the latter, open your .pcap in Wireshark to verify that valid packets are there, and also open the .bcf file in Notepad or another text editor to verify that the data has been converted correctly.\nThe .bcf should contain at least one line that begins with PHASE_1_CONFIG followed by at least two numbers.\nThe documentation contains an example of what a good .pcap looks like.\n\n");
        return -1;
    }

    //We don't want to run this function twice!
    if(configPattern.size()){
        printf("configPattern already contains %d elements!  Aborting...\n", configPattern.size());
        return 1;
    }
    initTransfer *currentTransfer = NULL;
    bool enteredDataStage = false;
    unsigned int currentLength;
    unsigned char *currentPayload;
    unsigned char transfer_type;
    unsigned char control_stage;
    unsigned char pseudoHeaderLength;
    rawPacket currentPacket;

    //The function needs to iterate over the packets in phase 1 only, stopping once we enter the data transfer stage.
    //Most of the offsets in the code below (e.g. transfer type being at position [22]) come from the structure of usbpcap's packets.
    //Open the .pcap file in Wireshark if you want to understand exactly what it is reading.

    for(int packetNumber = 0; packetNumber < phase1_raw->size(); packetNumber ++){
        currentPacket = phase1_raw->at(packetNumber);
        currentLength = currentPacket.length;
        currentPayload = currentPacket.dataPointer;
        pseudoHeaderLength = currentPayload[0];
        transfer_type = currentPayload[22];

        //Do not set the control_stage field for a BULK transfer.
        if(transfer_type==0x02){
            control_stage = currentPayload[27];
        }

        //Only the pointer to the data needs to be updated in the data stage.
        if((transfer_type == 0x02) && (control_stage == 0x01)){ //CONTROL-DATA
            currentTransfer->data = &currentPayload[pseudoHeaderLength];
            continue;
        }

        //Because I'm lazy, I'll assume that all status stages return "success".
        if((transfer_type == 0x02) && (control_stage == 0x02)){ //CONTROL-STATUS
            continue;
        }

        //A Bulk or (setup stage) control packet indicates a new USB transfer.
        currentTransfer = new initTransfer(phase1_interface);
        configPattern.push_back(currentTransfer);

        //These fields only need to be set once per transfer
        currentTransfer->direction_is_in = (currentPayload[21] & 0x80) ? 0x01 : 0x00;
        currentTransfer->endpoint = currentPayload[21] & 0x0f;
        currentTransfer->transfer_type = transfer_type;

        //Bulk packets have a slightly different structure to control packets, so if() statements are needed.
        if(transfer_type == 0x03){
            currentTransfer->data = &currentPayload[pseudoHeaderLength];
            currentTransfer->data_length = currentPayload[23] + 0x100*currentPayload[24] + 0x10000*currentPayload[25] + 0x1000000*currentPayload[26];
            //These values will be unused but it makes debugging clearer
            currentTransfer->bmRequestType = 69;
            currentTransfer->bRequest = 69;
            currentTransfer->wValue = 69;
            currentTransfer->wIndex = 69;
        }
        //There is an implicit assumption in the following if() statement that the control transfer is in the setup stage.
        //It should always be true because if it was not in the setup stage then it should be caught by one of the gotos above.
        if(transfer_type==0x02){ //CONTROL Only
            currentTransfer->bmRequestType = currentPayload[28];
            currentTransfer->bRequest = currentPayload[29];
            currentTransfer->wValue = currentPayload[30] + 0x100*currentPayload[31];
            currentTransfer->wIndex = currentPayload[32] + 0x100*currentPayload[33];
            currentTransfer->data_length = currentPayload[34] + 0x100*currentPayload[35];
        }
    }
    printf_verbose("configPattern.size() = %d\n", configPattern.size());
    return 0;
}

int phaseOneHandler::sendPattern(){
    int error;
    int bInterface = 0;
    std::vector<initTransfer*> *pattern = &configPattern;

    //Setup the USB device!
    if(phase1_interface->setup(bInterface)){
        //Error printing is handled internally by usbInterface::setup().
        return error;
    }

    //Actually send the packets!
    for(int i=0; i<pattern->size(); i++){
        error = pattern->at(i)->transmit();
        if(error)
        {
            fprintf(stderr, "Error sending transfer #%d.  Error code: 0x%08x; %s\n", i, error, libusb_error_name(error));
        } else printf_verbose("Transfer #%d sent successfully!\n", i);
    }
    return 0;
}
