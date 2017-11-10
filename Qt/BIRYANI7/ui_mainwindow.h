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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinBox;
    QPushButton *inspectButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *extractPreinitButton;
    QPushButton *sendPreinitButton;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *extractInitButton;
    QPushButton *sendInitButton;
    QPushButton *startDataStreamButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(400, 300);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        horizontalLayout->addWidget(spinBox);

        inspectButton = new QPushButton(centralWidget);
        inspectButton->setObjectName(QStringLiteral("inspectButton"));

        horizontalLayout->addWidget(inspectButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        extractPreinitButton = new QPushButton(centralWidget);
        extractPreinitButton->setObjectName(QStringLiteral("extractPreinitButton"));

        horizontalLayout_2->addWidget(extractPreinitButton);

        sendPreinitButton = new QPushButton(centralWidget);
        sendPreinitButton->setObjectName(QStringLiteral("sendPreinitButton"));

        horizontalLayout_2->addWidget(sendPreinitButton);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        extractInitButton = new QPushButton(centralWidget);
        extractInitButton->setObjectName(QStringLiteral("extractInitButton"));

        horizontalLayout_3->addWidget(extractInitButton);

        sendInitButton = new QPushButton(centralWidget);
        sendInitButton->setObjectName(QStringLiteral("sendInitButton"));

        horizontalLayout_3->addWidget(sendInitButton);


        verticalLayout->addLayout(horizontalLayout_3);

        startDataStreamButton = new QPushButton(centralWidget);
        startDataStreamButton->setObjectName(QStringLiteral("startDataStreamButton"));

        verticalLayout->addWidget(startDataStreamButton);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 21));
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
        inspectButton->setText(QApplication::translate("MainWindow", "Inspect", Q_NULLPTR));
        extractPreinitButton->setText(QApplication::translate("MainWindow", "Extract Preinit", Q_NULLPTR));
        sendPreinitButton->setText(QApplication::translate("MainWindow", "Send Preinit", Q_NULLPTR));
        extractInitButton->setText(QApplication::translate("MainWindow", "Extract Init", Q_NULLPTR));
        sendInitButton->setText(QApplication::translate("MainWindow", "Send Init", Q_NULLPTR));
        startDataStreamButton->setText(QApplication::translate("MainWindow", "Start Data Stream", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
