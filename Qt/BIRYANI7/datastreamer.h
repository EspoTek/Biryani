#ifndef DATASTREAMER_H
#define DATASTREAMER_H

#include <QWidget>
#include <QTimer>
#include <QDebug>

#include "userdefinedproperties.h"
#include "usbinterface.h"

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
    double extractedSamples[NUM_DATA_CHANNELS][LENGTH_DATA_PACKET/NUM_DATA_CHANNELS];
    void extractData(unsigned char* buffer);
    void drawBuffer(void);
signals:

public slots:
    void checkTimerTick(void);
};

#endif // DATASTREAMER_H
