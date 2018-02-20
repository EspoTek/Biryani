#include "phasezerohandler.h"
#include "libusb.h"

#include "api_defines.h"
#include "synamps_init.h"
#include "inittransfer.h"

phaseZeroHandler::phaseZeroHandler()
{
    phase0_interface = new usbInterface(SYNAMPS2_UNINITIALISED_VID, SYNAMPS2_UNINITIALISED_PID);
    if(phase0_interface == NULL){
        fprintf(stderr, "FATAL ERROR: phase1_interface is NULL\n");
    }
}

//Return values: -1 is unconnected, 0 is connected but not initialised, 1 is connected and initialised
int phaseZeroHandler::checkIfAlreadyInitialised(){
    //Variables
    libusb_context *ctx = NULL;
    libusb_device ** list;
    libusb_device *current_device;
    libusb_device_descriptor current_descriptor;
    int error;
    int num_usb_devices;
    uint16_t current_vid, current_pid;
    int synamps2_connection_state = -1;

    //Initialise libusb.
    error = libusb_init(&ctx);
    if(error){
        fprintf(stderr, "Count not initialise libusb.  Error code %08x\n", error);
        return error;
    }

    libusb_set_debug(ctx, debug_level/100);

    //Initialise the device list.
    num_usb_devices = libusb_get_device_list(ctx, &list);
    printf_verbose("num_usb_devices = %d\n", num_usb_devices);

    //Iterate over the list and search for the "Synamps 2 (Uninitialised)" device
    for(int i=0; i<num_usb_devices; i++){
        current_device = list[i];
        if(current_device == NULL){
            printf("Reached end of list\n");
            continue;
        } //else

        libusb_get_device_descriptor(current_device, &current_descriptor);
        current_vid = current_descriptor.idVendor;
        current_pid = current_descriptor.idProduct;
        printf_verbose("\nDevice %d\nVID=0x%04x\nPID=0x%04x\n", i, current_vid, current_pid);
        if(current_vid == SYNAMPS2_UNINITIALISED_VID && current_pid == SYNAMPS2_UNINITIALISED_PID){
            printf_verbose("Synamps2 (uninitialised) device\n");
            synamps2_connection_state = 0;
        }  else if (current_vid == SYNAMPS2_MAIN_VID && current_pid == SYNAMPS2_MAIN_PID){
            printf_verbose("Synamps2 (main) device\n");
            synamps2_connection_state = 1;
        } else {
            printf_verbose("Other device\n");
        }
    }

    //Free the device list and exit the context.
    libusb_free_device_list(list, 1);
    libusb_exit(ctx);

    return synamps2_connection_state;
}

//The Synamps2 device actually enumerates as two separate USB devices: 0B6E/0005 and 0B6E/0006.
//When you first turn it on, it will enumerate as 0B6E/0005.
//You then need to send some configuration or firmware (?) data to the device using vendor command 0xa0.
//After this is done, it will enumerate as 0B6E/0006 and be ready for initialisation.
//This function creates a chain of packets that sends the aforementioned configuration/firmware data.
int phaseZeroHandler::createPattern(void){
    printf_verbose("phaseZeroHandler::createInitPattern()\n");

    //We don't want to run this function twice!
    if(initPattern.size()){
        printf_verbose("initPattern already contains %llu elements!  Returning...\n", initPattern.size());
        return 1;
    }

    initTransfer *currentTransfer = NULL;
    unsigned int packetNumber = 0;
    bool enteredDataStage = false;
    unsigned int packetLength;
    const unsigned char *packet;
    unsigned char transfer_type;
    unsigned char control_stage;
    unsigned char pseudoHeaderLength;
    bool current_packet_is_0xa0 = false;

    //This function needs to iterate over all packets.
    //Those that are not control-vendor command 0xa0 should be ignored.
    for(packetNumber = 0; packetNumber<NUM_WIRESHARK_PACKETS_phase0; packetNumber++){
        packetLength = wireshark_packet_lengths_phase0[packetNumber];
        packet = wireshark_packets_phase0[packetNumber];
        pseudoHeaderLength = packet[0];
        transfer_type = packet[22];

        if(transfer_type==0x03){
            //We're not interested in bulk packets because they're not vendor 0xa0.
            continue;
        }

        control_stage = packet[27];

        //Only the pointer to the data needs to be updated in the data stage.
        if((transfer_type == 0x02) && (control_stage == 0x01) && current_packet_is_0xa0){ //CONTROL-DATA
            currentTransfer->data = &packet[pseudoHeaderLength];
            continue;
        }

        //Because I'm lazy, I'll assume that all status stages return "success".
        if((transfer_type == 0x02) && (control_stage == 0x02)){ //CONTROL-STATUS
            current_packet_is_0xa0 = false;
            continue;
        }

        //If neither of the above two if()s triggered, we're looking at a setup stage control packet.
        //Now all that's left to check is that the command is vendor-0xa0
        //First condition checks bmRequestType to ensure it's vendor; second checks that it's of type 0xa0
        if(((packet[28] & 0x60) != 0x40) || (packet[29] != 0xa0)){
            if((packet[28] & 0x60) != 0x40) printf_verbose("Packet %d is not of type vendor!\n", packetNumber);
            if(packet[29] != 0xa0) printf_verbose("Packet %d does not have bRequest of 0xa0\n", packetNumber);
            continue;
        }

        //If it's reached this stage, it's the setup stage for a control transfer of vendor type 0xa0.
        //Lets allocate it's packet!
        currentTransfer = new initTransfer(phase0_interface);
        initPattern.push_back(currentTransfer);
        current_packet_is_0xa0 = true;

        //These fields only need to be set once per transfer, during the setup stage
        currentTransfer->direction_is_in = (packet[21] & 0x80) ? 0x01 : 0x00;
        currentTransfer->endpoint = packet[21] & 0x0f;
        currentTransfer->transfer_type = transfer_type;
        currentTransfer->bmRequestType = packet[28];
        currentTransfer->bRequest = packet[29];
        currentTransfer->wValue = packet[30] + 0x100*packet[31];
        currentTransfer->wIndex = packet[32] + 0x100*packet[33];
        currentTransfer->data_length = packet[34] + 0x100*packet[35];
    }
    printf_verbose("initPattern.size() = %llu\n", initPattern.size());

    return 0;
}

int phaseZeroHandler::sendPattern(){
    int error;
    int bInterface = 0;
    std::vector<initTransfer*> *pattern = &initPattern;

    //Setup the USB device!
	error = phase0_interface->setup(bInterface);
    if(error){
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

