#include "streamingdisplay.h"
#include "ui_streamingdisplay.h"

streamingDisplay::streamingDisplay(QWidget *parent, usbInterface *interface) :
    QMainWindow(parent),
    ui(new Ui::streamingDisplay)
{
    ui->setupUi(this);
    streamHandler = new dataStreamer(this, interface);
}

streamingDisplay::~streamingDisplay()
{
    delete ui;
    delete(streamHandler);
}


void streamingDisplay::closeEvent( QCloseEvent *event ){
    delete(this);
}
