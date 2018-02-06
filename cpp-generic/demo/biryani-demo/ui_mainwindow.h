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

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *debugLevelLabel;
    QSpinBox *debugLevelSspinBox;
    QPushButton *loadFileButton;
    QPushButton *initialiseAmplifierButton;
    QPushButton *configureAmplifierButton;
    QPushButton *testActionButton;
    QPushButton *testAction2Button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(540, 360);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
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

        testActionButton = new QPushButton(centralWidget);
        testActionButton->setObjectName(QStringLiteral("testActionButton"));

        verticalLayout->addWidget(testActionButton);

        testAction2Button = new QPushButton(centralWidget);
        testAction2Button->setObjectName(QStringLiteral("testAction2Button"));

        verticalLayout->addWidget(testAction2Button);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 540, 21));
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
        debugLevelLabel->setText(QApplication::translate("MainWindow", "Debug Level", Q_NULLPTR));
        loadFileButton->setText(QApplication::translate("MainWindow", "Load File", Q_NULLPTR));
        initialiseAmplifierButton->setText(QApplication::translate("MainWindow", "Initialise Amplifier", Q_NULLPTR));
        configureAmplifierButton->setText(QApplication::translate("MainWindow", "Configure Amplifier", Q_NULLPTR));
        testActionButton->setText(QApplication::translate("MainWindow", "Test Action", Q_NULLPTR));
        testAction2Button->setText(QApplication::translate("MainWindow", "Test Action 2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
