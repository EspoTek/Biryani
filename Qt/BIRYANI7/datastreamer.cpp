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

    plot_xaxis.resize(MAX_CONVERTED_SAMPLES_STORED);
    for (int i=0;i<NUM_DATA_CHANNELS;i++){
        xaxis_array[i] = &plot_xaxis;
    }

}

dataStreamer::~dataStreamer(){
    checkTimer->stop();
    delete(checkTimer);
}

//This is called every CHECK_INTERVAL milliseconds.
void dataStreamer::checkTimerTick(void){
    static int tickCount = 0;
    tickCount++;
    qDebug() << "checkTimerTick" << tickCount;
    int bytes_transferred;
    //Synchronous call, need to draw immediately after.
    libusb_interface->transfer_bulk(true, 0x06, buffer, buffer, LENGTH_DATA_PACKET, &bytes_transferred);
    if(bytes_transferred){
        extractData(buffer);
        drawBuffer();
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
    internalPacketStash->decodePacket();
    qDebug() << "ready?" << internalPacketStash->ready();
}

void dataStreamer::drawBuffer(){
    qDebug() << "dataStreamer::drawBuffer()";
    double sampleRate = 2000;
    int mode = 0;
    double delay = 0.01;
    double timeWindow = 1;
    int length;
    QVector<double> **decodedPacketStreams = internalPacketStash->getDownSampledChannelData(sampleRate, mode, delay, timeWindow, &length);

    //Most recent sample is the zeroth element.
    for (int i=0;i<length;i++){
        plot_xaxis.replace(i, -delay-timeWindow+(((double)i)/sampleRate));
    }
    pleaseDraw(xaxis_array, decodedPacketStreams, length, NUM_DATA_CHANNELS);
}
