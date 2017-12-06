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
    QVector<double> plot_xaxis;
    QVector<double> *xaxis_array[NUM_DATA_CHANNELS];
signals:
    void pleaseDraw(QVector<double>** x, QVector<double>** y, int numSamples, int numChannels);

public slots:
    void checkTimerTick(void);
};

#endif // DATASTREAMER_H
