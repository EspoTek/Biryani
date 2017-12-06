#ifndef DATASTREAMER_H
#define DATASTREAMER_H

#include <QWidget>
#include <QTimer>
#include <QDebug>

#include "userdefinedproperties.h"
#include "usbinterface.h"

class packetBuffer;

//Number of ms between checks for new data
#define CHECK_INTERVAL 1

class dataStreamer : public QWidget
{
    Q_OBJECT
public:
    explicit dataStreamer(QWidget *parent = 0, usbInterface* interface = NULL);
    ~dataStreamer();
private:
    QTimer *checkTimer = NULL;
    bool drawNext = false;
    usbInterface *libusb_interface;
    void extractData(unsigned char* buffer);
    void drawBuffer(void);
    unsigned char *buffer;
    packetBuffer *internalPacketStash;
    QVector<double> *decodedPacketStreams[NUM_DATA_CHANNELS];
signals:

public slots:
    void checkTimerTick(void);
};

#endif // DATASTREAMER_H
