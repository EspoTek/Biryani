#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::MainWindow *ui;
    apiInterface *generic_api;
};

#endif // MAINWINDOW_H
