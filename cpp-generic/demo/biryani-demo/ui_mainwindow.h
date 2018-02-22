/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QSpinBox *channelPlotSpinBox;
    QHBoxLayout *horizontalLayout;
    QLabel *debugLevelLabel;
    QSpinBox *debugLevelSspinBox;
    QPushButton *loadFileButton;
    QPushButton *initialiseAmplifierButton;
    QPushButton *configureAmplifierButton;
    QPushButton *startStreamButton;
    QPushButton *stopStreamButton;
    QPushButton *testActionButton;
    QPushButton *testAction2Button;
    QPushButton *getAverageLatencyButton;
    QPushButton *plotTestButton;
    QPushButton *getPacketIntervalButton;
    QPushButton *getSampleRateButton;
    QPushButton *getDataSinceLastCallButton;
    QPushButton *getAllChannelsButton;
    QCustomPlot *plotAxes;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(890, 472);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_2 = new QHBoxLayout(centralWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        channelPlotSpinBox = new QSpinBox(centralWidget);
        channelPlotSpinBox->setObjectName(QStringLiteral("channelPlotSpinBox"));
        channelPlotSpinBox->setMaximum(67);

        horizontalLayout_3->addWidget(channelPlotSpinBox);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        debugLevelLabel = new QLabel(centralWidget);
        debugLevelLabel->setObjectName(QStringLiteral("debugLevelLabel"));

        horizontalLayout->addWidget(debugLevelLabel);

        debugLevelSspinBox = new QSpinBox(centralWidget);
        debugLevelSspinBox->setObjectName(QStringLiteral("debugLevelSspinBox"));
        debugLevelSspinBox->setMaximum(1000);

        horizontalLayout->addWidget(debugLevelSspinBox);


        verticalLayout->addLayout(horizontalLayout);

        loadFileButton = new QPushButton(centralWidget);
        loadFileButton->setObjectName(QStringLiteral("loadFileButton"));

        verticalLayout->addWidget(loadFileButton);

        initialiseAmplifierButton = new QPushButton(centralWidget);
        initialiseAmplifierButton->setObjectName(QStringLiteral("initialiseAmplifierButton"));

        verticalLayout->addWidget(initialiseAmplifierButton);

        configureAmplifierButton = new QPushButton(centralWidget);
        configureAmplifierButton->setObjectName(QStringLiteral("configureAmplifierButton"));

        verticalLayout->addWidget(configureAmplifierButton);

        startStreamButton = new QPushButton(centralWidget);
        startStreamButton->setObjectName(QStringLiteral("startStreamButton"));

        verticalLayout->addWidget(startStreamButton);

        stopStreamButton = new QPushButton(centralWidget);
        stopStreamButton->setObjectName(QStringLiteral("stopStreamButton"));

        verticalLayout->addWidget(stopStreamButton);

        testActionButton = new QPushButton(centralWidget);
        testActionButton->setObjectName(QStringLiteral("testActionButton"));

        verticalLayout->addWidget(testActionButton);

        testAction2Button = new QPushButton(centralWidget);
        testAction2Button->setObjectName(QStringLiteral("testAction2Button"));

        verticalLayout->addWidget(testAction2Button);

        getAverageLatencyButton = new QPushButton(centralWidget);
        getAverageLatencyButton->setObjectName(QStringLiteral("getAverageLatencyButton"));

        verticalLayout->addWidget(getAverageLatencyButton);

        plotTestButton = new QPushButton(centralWidget);
        plotTestButton->setObjectName(QStringLiteral("plotTestButton"));

        verticalLayout->addWidget(plotTestButton);

        getPacketIntervalButton = new QPushButton(centralWidget);
        getPacketIntervalButton->setObjectName(QStringLiteral("getPacketIntervalButton"));

        verticalLayout->addWidget(getPacketIntervalButton);

        getSampleRateButton = new QPushButton(centralWidget);
        getSampleRateButton->setObjectName(QStringLiteral("getSampleRateButton"));

        verticalLayout->addWidget(getSampleRateButton);

        getDataSinceLastCallButton = new QPushButton(centralWidget);
        getDataSinceLastCallButton->setObjectName(QStringLiteral("getDataSinceLastCallButton"));

        verticalLayout->addWidget(getDataSinceLastCallButton);

        getAllChannelsButton = new QPushButton(centralWidget);
        getAllChannelsButton->setObjectName(QStringLiteral("getAllChannelsButton"));

        verticalLayout->addWidget(getAllChannelsButton);


        horizontalLayout_2->addLayout(verticalLayout);

        plotAxes = new QCustomPlot(centralWidget);
        plotAxes->setObjectName(QStringLiteral("plotAxes"));

        horizontalLayout_2->addWidget(plotAxes);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 890, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Channel to Plot", Q_NULLPTR));
        debugLevelLabel->setText(QApplication::translate("MainWindow", "Debug Level", Q_NULLPTR));
        loadFileButton->setText(QApplication::translate("MainWindow", "Load File", Q_NULLPTR));
        initialiseAmplifierButton->setText(QApplication::translate("MainWindow", "Initialise Amplifier", Q_NULLPTR));
        configureAmplifierButton->setText(QApplication::translate("MainWindow", "Configure Amplifier", Q_NULLPTR));
        startStreamButton->setText(QApplication::translate("MainWindow", "Start Stream", Q_NULLPTR));
        stopStreamButton->setText(QApplication::translate("MainWindow", "Stop Stream", Q_NULLPTR));
        testActionButton->setText(QApplication::translate("MainWindow", "Test Action", Q_NULLPTR));
        testAction2Button->setText(QApplication::translate("MainWindow", "Test Action 2", Q_NULLPTR));
        getAverageLatencyButton->setText(QApplication::translate("MainWindow", "Get Average Latency", Q_NULLPTR));
        plotTestButton->setText(QApplication::translate("MainWindow", "Start Plotting Test", Q_NULLPTR));
        getPacketIntervalButton->setText(QApplication::translate("MainWindow", "Get Packet Interval", Q_NULLPTR));
        getSampleRateButton->setText(QApplication::translate("MainWindow", "Get Sample Rate", Q_NULLPTR));
        getDataSinceLastCallButton->setText(QApplication::translate("MainWindow", "Get Data Since Last Call", Q_NULLPTR));
        getAllChannelsButton->setText(QApplication::translate("MainWindow", "Get All Channels", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
