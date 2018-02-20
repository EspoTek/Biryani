#ifndef INITPACKET_H
#define INITPACKET_H

#include "usbinterface.h"

class initTransfer
{
public:
    initTransfer(usbInterface *interface);
    ~initTransfer();
    int transmit();
    usbInterface *libusb_interface = NULL;
    unsigned char direction_is_in;
    unsigned char endpoint;
    unsigned char transfer_type;
    const unsigned char *data;
    unsigned int data_length; //bytes

    //Control only
    unsigned char bmRequestType;
    unsigned char bRequest;
    unsigned short wValue;
    unsigned short wIndex;
};

#endif // INITPACKET_H
