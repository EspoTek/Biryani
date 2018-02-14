#include "mainwindow.h"
#include "ui_mainwindow.h"

//Qt includes
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>

//Generic Includes
#include "apiinterface.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    generic_api = new apiInterface();



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
    ui->plotAxes->addGraph();
    ui->plotAxes->graph(0)->setData(x, y);
    // give the axes some labels:
    ui->plotAxes->xAxis->setLabel("x");
    ui->plotAxes->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    ui->plotAxes->xAxis->setRange(-1, 1);
    ui->plotAxes->yAxis->setRange(0, 1);
    ui->plotAxes->replot();

    //Hide deprecated buttons
    ui->configureAmplifierButton->setVisible(false);
    ui->testActionButton->setVisible(false);
    ui->testAction2Button->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFileButton_clicked()
{
    qDebug() << "clicked!";
    QFileDialog dialog;

    dialog.setDefaultSuffix(".bcf");
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("Biryani Configuration Files (*.bcf);;All Files (*)");

    int retVal = dialog.exec();

    if(!retVal){
        qDebug() << "No file selected";
        return;
    }

    QStringList tempList = dialog.selectedFiles();
    QString temp = tempList.first();
    char *fileName = temp.toLocal8Bit().data();

    generic_api->loadFile(fileName);
}

void MainWindow::on_testActionButton_clicked()
{
    generic_api->testAction();
}

void MainWindow::on_initialiseAmplifierButton_clicked()
{
    generic_api->initialiseSynamps2Device();
}

void MainWindow::on_debugLevelSspinBox_valueChanged(int arg1)
{
    generic_api->setDebugLevel(arg1);
}

void MainWindow::on_configureAmplifierButton_clicked()
{
    //generic_api->configureSynamps2Device();
}

void MainWindow::on_testAction2Button_clicked()
{
    generic_api->testAction_2();
}

void MainWindow::on_stopStreamButton_clicked()
{
    generic_api->stopStream();
}

void MainWindow::on_startStreamButton_clicked()
{
    generic_api->startStream();
}

void MainWindow::on_plotTestButton_clicked()
{
    plotTimer = new QTimer();
    plotTimer->setTimerType(Qt::PreciseTimer);
    plotTimer->start(30);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(plotTimerTick()));
}

void MainWindow::plotTimerTick(){
    qDebug() << "MainWindow::plotTimerTick()";
    double sampleRate = 2000;
    int filter_mode = 0;
    double delay = 0.01;
    double timeWindow = 0.05;
    int length;

    std::vector<double> *samples = generic_api->getDownSampledChannelData_double(current_plotting_channel, sampleRate, filter_mode, delay, timeWindow, &length);

    if(samples == NULL){
        qDebug() << "No samples.  Returning...";
        return;
    }

    QVector<double> yaxis = QVector<double>::fromStdVector(*(samples));
    for (int i=0; i<yaxis.length(); i++){
        if(yaxis.at(i) > yMax){
            yMax = yaxis.at(i) + 1;// * (yaxis.at(i) > 0 ? 1.25 : 0.8);
        }

        if(yaxis.at(i) < yMin){
            yMin = yaxis.at(i) - 1;// * (yaxis.at(i) > 0 ? 0.8 : 1.25);
        }
    }

    //qDebug() << "yMax =" << yMax;
    //qDebug() << "yMin =" << yMin;
    //qDebug() << "yaxis.at(1) =" << yaxis.at(1);

    //qDebug() << yaxis.length();

    double sampleInterval = timeWindow/yaxis.length();

    QVector<double> xaxis;
    for (int i=0; i<yaxis.length(); i++){
        xaxis.append(-sampleInterval * i - delay);
    }

    //qDebug() << "xaxis ready";

    ui->plotAxes->yAxis->setRange(yMax, yMin);
    ui->plotAxes->xAxis->setRange(-delay - timeWindow, -delay);

    //qDebug() << "Range set";

    ui->plotAxes->graph(0)->setData(xaxis, yaxis);
    ui->plotAxes->replot();

    //qDebug() << "Tick returning";
}

void MainWindow::on_channelPlotSpinBox_valueChanged(int arg1)
{
    if(arg1 <= generic_api->getNumChannelsExcludingRef()){
        current_plotting_channel = arg1;
    } else {
        ui->channelPlotSpinBox->setValue(current_plotting_channel);
    }

    yMax = -100000000000;
    yMin = 100000000000;
}
