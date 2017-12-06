#ifndef PACKETBUFFER_H
#define PACKETBUFFER_H

#include <QObject>
#include "userdefinedproperties.h"

#define NUM_PACKETS_KEPT 3
#define MAX_PACKET_INDEX (NUM_PACKETS_KEPT-1)

class o1buffer;

class packetBuffer : public QObject
{
    Q_OBJECT
public:
    explicit packetBuffer(QObject *parent = 0);
    void addPacket(unsigned char* packetBuffer);
    unsigned char* getPacket(unsigned int position);
    bool ready();
    o1buffer *buffer_CH[NUM_DATA_CHANNELS];
    QVector<double>** getDownSampledChannelData(double sampleRate_hz, int mode, double delay_seconds, double timeWindow_seconds, int* length);
    void decodePacket();
private:
    unsigned char storageBuffer[LENGTH_DATA_PACKET][NUM_PACKETS_KEPT];
    unsigned int nextPacketIndex = 0;
    bool startingPacketFound = false;
    unsigned int lastPacketIndex();
    unsigned int packetStartOffset = 0;
    unsigned char *interPacketBuffer;
    subPacket *tempSubPacket;
    QVector<double>* sampledPacketStreams[NUM_DATA_CHANNELS];
signals:

public slots:
};

#endif // PACKETBUFFER_H
