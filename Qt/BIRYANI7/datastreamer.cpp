#include "datastreamer.h"
#include "packetbuffer.h"

//dataStreamer is a "manager" object that gets data from the usbInterface, stores it away in the o1buffer and then plots it using qCustomPlot.
dataStreamer::dataStreamer(QWidget *parent, usbInterface *interface) : QWidget(parent)
{
    if(interface == NULL){
        qFatal("dataStreamer is being initialised with a null libsub interface!!!");
    }

    //Ensure this is set up before creating the dataStreamer
    libusb_interface = interface;

    //Set up the o1buffer
    internalPacketStash = new packetBuffer();

    checkTimer = new QTimer;
    checkTimer->setTimerType(Qt::PreciseTimer);
    checkTimer->start(CHECK_INTERVAL);
    connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkTimerTick()));
    buffer = (unsigned char *) malloc(LENGTH_DATA_PACKET);
}

dataStreamer::~dataStreamer(){
    checkTimer->stop();
    delete(checkTimer);
}

//This is called every CHECK_INTERVAL milliseconds.
void dataStreamer::checkTimerTick(void){
    qDebug() << "checkTimerTick";
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

//extractData gets the packet from the usbInterface, and stashes it away in the o1buffer for safekeeping.
void dataStreamer::extractData(unsigned char *buffer){
    qDebug() << "dataStreamer::extractData(unsigned char *buffer)";
    internalPacketStash->addPacket(buffer);
}

void dataStreamer::drawBuffer(){
    qDebug() << "dataStreamer::drawBuffer()";
}
