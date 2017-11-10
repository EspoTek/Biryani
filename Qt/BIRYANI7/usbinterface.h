#ifndef USBINTERFACE_H
#define USBINTERFACE_H

#include "libusb.h"

class usbInterface
{
public:
    usbInterface(unsigned short vendor_id, unsigned short product_id);
    int setup(int bInterface);
    int transfer_control(bool dir_is_in, const unsigned char *expected_data, unsigned char *buffer, unsigned char bmRequestType, unsigned char bRequest, unsigned short wValue, unsigned short wIndex, unsigned short wLength);
    int transfer_bulk(bool dir_is_in, unsigned char endpoint, const unsigned char *expected_data, unsigned char *buffer, unsigned int data_length, int *transferred);
    int transfer_compare(const unsigned char *constBuffer, unsigned char *variableBuffer, unsigned int length);
private:
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;
    unsigned short vid, pid;
};

#endif // USBINTERFACE_H
