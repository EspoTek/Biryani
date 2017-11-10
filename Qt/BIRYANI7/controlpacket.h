#ifndef CONTROLPACKET_H
#define CONTROLPACKET_H


class controlPacket : public initPacket
{
public:
    controlPacket();
private:
    unsigned char bmRequestType;
    unsigned char bRequest;
    unsigned short wValue;
    unsigned short wIndex;
    unsigned short wLength;
};

#endif // CONTROLPACKET_H
