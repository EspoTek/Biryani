#include "datastreamer.h"

dataStreamer::dataStreamer(QWidget *parent, usbInterface *interface) : QWidget(parent)
{
    if(interface == NULL){
        qFatal("dataStreamer is being initialised with a null libsub interface!!!");
    }

    //Ensure this is set up before creating the dataStreamer
    libusb_interface = interface;

    checkTimer = new QTimer;
    checkTimer->setTimerType(Qt::PreciseTimer);
    checkTimer->start(CHECK_INTERVAL);
    connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkTimerTick()));
}

dataStreamer::~dataStreamer(){
    checkTimer->stop();
    delete(checkTimer);
}

void dataStreamer::checkTimerTick(void){
    qDebug() << "checkTimerTick";
    unsigned char *buffer = (unsigned char *) malloc(LENGTH_DATA_PACKET);
    int bytes_transferred;
    libusb_interface->transfer_bulk(true, 0x06, buffer, buffer, LENGTH_DATA_PACKET, &bytes_transferred);
    if(bytes_transferred){
        extractData(buffer);
        drawNext = true;
    }
    if((bytes_transferred == 0) && drawNext){
        drawBuffer();
        drawNext = false;
    }
}

void dataStreamer::extractData(unsigned char *buffer){
    qDebug() << "dataStreamer::extractData(unsigned char *buffer)";
}

void dataStreamer::drawBuffer(){
    qDebug() << "dataStreamer::drawBuffer()";
}
