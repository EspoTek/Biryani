/********************************************************************************
** Form generated from reading UI file 'streamingdisplay.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STREAMINGDISPLAY_H
#define UI_STREAMINGDISPLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_streamingDisplay
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QCustomPlot *widget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *streamingDisplay)
    {
        if (streamingDisplay->objectName().isEmpty())
            streamingDisplay->setObjectName(QStringLiteral("streamingDisplay"));
        streamingDisplay->resize(800, 600);
        centralwidget = new QWidget(streamingDisplay);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        widget = new QCustomPlot(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));

        verticalLayout->addWidget(widget);

        streamingDisplay->setCentralWidget(centralwidget);
        menubar = new QMenuBar(streamingDisplay);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        streamingDisplay->setMenuBar(menubar);
        statusbar = new QStatusBar(streamingDisplay);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        streamingDisplay->setStatusBar(statusbar);

        retranslateUi(streamingDisplay);

        QMetaObject::connectSlotsByName(streamingDisplay);
    } // setupUi

    void retranslateUi(QMainWindow *streamingDisplay)
    {
        streamingDisplay->setWindowTitle(QApplication::translate("streamingDisplay", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class streamingDisplay: public Ui_streamingDisplay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STREAMINGDISPLAY_H
