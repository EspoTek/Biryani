#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
class apiInterface;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadFileButton_clicked();

    void on_testActionButton_clicked();

    void on_initialiseAmplifierButton_clicked();

    void on_debugLevelSspinBox_valueChanged(int arg1);

    void on_configureAmplifierButton_clicked();

    void on_testAction2Button_clicked();

    void on_stopStreamButton_clicked();

    void on_startStreamButton_clicked();

    void on_plotTestButton_clicked();

    void plotTimerTick();
    void on_channelPlotSpinBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    apiInterface *generic_api;
    QTimer *plotTimer;
    int current_plotting_channel = 0;
    double yMin = 100000000000, yMax = -100000000000;
};

#endif // MAINWINDOW_H
