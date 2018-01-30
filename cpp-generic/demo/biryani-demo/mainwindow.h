#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class configurationFileHandler;

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

private:
    Ui::MainWindow *ui;
    configurationFileHandler *cfgHandler;
};

#endif // MAINWINDOW_H
