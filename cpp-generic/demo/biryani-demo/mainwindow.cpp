#include "mainwindow.h"
#include "ui_mainwindow.h"

//Qt includes
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>

//Generic Includes
#include "configurationfilehandler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cfgHandler = new configurationFileHandler();
    setbuf(stdout, NULL);
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
    }

    QStringList tempList = dialog.selectedFiles();
    QString temp = tempList.first();
    char *fileName = temp.toLocal8Bit().data();

    cfgHandler->loadFile(fileName);
}
