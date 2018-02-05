#include "usbinterface.h"

//usbInterface handles the more "to the metal" aspects of sending the transfers over USB.
//Basically, this means wrapping around libusb and ensuring the right parts of the transfer are sent to the right function.

usbInterface::usbInterface(unsigned short vendor_id, unsigned short product_id)
{
    vid = vendor_id;
    pid = product_id;

    printf("usbInterface created.  VID = %04x; PID = %04x\n", vid, pid);
}

//This sets up the libusb environment and opens the device.
//It must be called once before you can transmit or receive any data to/from the Synamps2 device.
int usbInterface::setup(int bInterface){
    int error;

    //Setup() only needs to run once.
    if(handle!=NULL){
        printf("usbInterface::setup() called, but interface is already set up!\n");
        return 0;
    }

    //Initialise libusb.
    error = libusb_init(&ctx);
    if(error){
        fprintf(stderr, "Count not initialise libusb.  Error code %08x\n", error);
        return error;
    }

    libusb_set_debug(ctx, debug_level/100);

    //Open the device and get a handle for it.
    //Note that this implementation can't handle multiple headboxes.
    handle = libusb_open_device_with_vid_pid(ctx, vid, pid);
    if(handle==NULL){
        fprintf(stderr, "Count not open device %04x/%04x.\nIs the headbox connected and preinitialised?\nIs the driver installed?\n", vid, pid);
        return -69; //Error: handle null
    }

    error = libusb_claim_interface(handle, bInterface);
    if(error){
        fprintf(stderr, "Count not claim interface.  Error code %08x", error);
        return error;
    }

    return 0;
}

//This function sends a control transfer over the USB bus (Universal Serial Bus bus).
int usbInterface::transfer_control(bool dir_is_in, const unsigned char *expected_data, unsigned char *buffer, unsigned char bmRequestType, unsigned char bRequest, unsigned short wValue, unsigned short wIndex, unsigned short wLength){
    int error;

    unsigned int timeout = 2000;

    //The actual call to libusb
    printf_verbose("libusb_control_transfer(handle, %02x, %02x, %04x, %04x, buffer, %04x, %u\n)", bmRequestType, bRequest, wValue, wIndex, wLength, timeout);
    error = libusb_control_transfer(handle, bmRequestType, bRequest, wValue, wIndex, buffer, wLength, timeout);
    if(error) return error;

    //If the device sent data to the PC, check that it matches what we expect.
    if(dir_is_in){
        return transfer_compare(expected_data, buffer, wLength);
    }
    return 0;
}

//This function sends a bulk transfer over the USB bus (Universal Serial Bus bus).
int usbInterface::transfer_bulk(bool dir_is_in, unsigned char endpoint, const unsigned char *expected_data, unsigned char *buffer, unsigned int data_length, int *transferred){
    int error;

    unsigned int timeout = 2000;

    printf_verbose("libusb_bulk_transfer(handle, 0x%02x, buffer, %u, transferred, %u\n);", (dir_is_in ? (0x80 | endpoint) : endpoint), data_length, timeout);
    error = libusb_bulk_transfer(handle, (dir_is_in ? (0x80 | endpoint) : endpoint), buffer, (int)data_length, transferred, timeout);

    if(*(transferred) != data_length){
        printf("WARNING: Expected transfer of length %u, actually got length of %d\n", data_length, *(transferred));
        return -69;
    }
    if(dir_is_in){
        return transfer_compare(expected_data, buffer, data_length);
    }
    return 0;
}

//This function compares two arrays to ensure they contain the same data.
//Returns 0 if they do, otherwise returns 0x0C05FEFE.
int usbInterface::transfer_compare(const unsigned char *constBuffer, unsigned char *variableBuffer, unsigned int length){
    //Return early if the two buffers share the same location.  No need to compare.
    if(constBuffer == variableBuffer) return 0;

    for (int i=0;i<length;i++)
    {
        if(constBuffer[i] != variableBuffer[i])
        {
            return 0x0C05FEFE; //Error: covfefe not found
        }
    }
    return 0;
}
