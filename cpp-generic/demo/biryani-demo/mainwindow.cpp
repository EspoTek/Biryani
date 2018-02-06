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
    generic_api->configureSynamps2Device();
}

void MainWindow::on_testAction2Button_clicked()
{
    generic_api->testAction_2();
}
