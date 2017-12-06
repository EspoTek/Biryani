#include "streamingdisplay.h"
#include "ui_streamingdisplay.h"

streamingDisplay::streamingDisplay(QWidget *parent, usbInterface *interface) :
    QMainWindow(parent),
    ui(new Ui::streamingDisplay)
{
    ui->setupUi(this);
    streamHandler = new dataStreamer(this, interface);
    connect(streamHandler, SIGNAL(pleaseDraw(QVector<double>**, QVector<double>**, int, int)), this, SLOT(drawThis(QVector<double>**, QVector<double>**, int, int)));


    //Just the default QCP setup.
    //http://www.qcustomplot.com/index.php/tutorials/basicplotting
    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i/50.0 - 1; // x goes from -1 to 1
      y[i] = x[i]*x[i]; // let's plot a quadratic function
    }
    // create graph and assign data to it:
    ui->qcpWidget->addGraph();
    ui->qcpWidget->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->qcpWidget->xAxis->setLabel("x");
    ui->qcpWidget->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->qcpWidget->xAxis->setRange(-1, 1);
    ui->qcpWidget->yAxis->setRange(0, 1);
    ui->qcpWidget->replot();
}

streamingDisplay::~streamingDisplay()
{
    delete ui;
    delete(streamHandler);
}


void streamingDisplay::closeEvent( QCloseEvent *event ){
    delete(this);
}

void streamingDisplay::drawThis(QVector<double>** x, QVector<double>** y, int numSamples, int numChannels){
    qDebug() << "streamingDisplay::drawThis";
    QVector<double> a(101);
    QVector<double> b(101);

    //ui->qcpWidget->graph(0)->setData(a, b);
    ui->qcpWidget->graph(0)->setData(*(x)[0], *(y)[0]);
    ui->qcpWidget->xAxis->setRange(-1.01, -0.02);
    ui->qcpWidget->yAxis->setRange(0, 10000000);
    ui->qcpWidget->replot();

}
