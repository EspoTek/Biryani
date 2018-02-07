#include "inittransfer.h"

//initTransfer is a class that holds information about a single USB transfer used to initialise (or preinitialise) the Synamps2 device.
//There's also a method to initiate the transfer.

initTransfer::initTransfer(usbInterface *interface)
{
    libusb_interface = interface;
}

//This function will prompt the usbInterface to transfer the packet in the appropriate way
int initTransfer::transmit(){
    printf_debugging("initTransfer::transmit()\n");
    int error;
    const unsigned char *expected_data = NULL;
    unsigned char *buffer = (unsigned char*) malloc(data_length);

    //If the packet is coming in (from device to the PC), we can check to see if it matches what we expect.
    if(direction_is_in){
        expected_data = data;
    } else {
        memcpy(buffer, data, data_length);
    }

    //Call the appropriate method to send transfer over USB.
    switch(transfer_type){
        case 0x02: //Control
            error = libusb_interface->transfer_control(direction_is_in, expected_data, buffer, bmRequestType, bRequest, wValue, wIndex, data_length);
            break;
        case 0x03:
            int transferred;
            error = libusb_interface->transfer_bulk(direction_is_in, endpoint, expected_data, buffer, data_length, &transferred);
            break;
        default:
            fprintf(stderr, "initPacket::transmit() does not know how to handle transfers of type 0x%02x!", transfer_type);
    }

    free(buffer);
    return error;
}
