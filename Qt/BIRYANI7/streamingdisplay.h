#ifndef STREAMINGDISPLAY_H
#define STREAMINGDISPLAY_H

#include <QMainWindow>
#include <QDebug>

#include "datastreamer.h"

namespace Ui {
class streamingDisplay;
}

class streamingDisplay : public QMainWindow
{
    Q_OBJECT

public:
    explicit streamingDisplay(QWidget *parent = 0, usbInterface *interface = NULL);
    ~streamingDisplay();
    virtual void closeEvent ( QCloseEvent * event );

private:
    Ui::streamingDisplay *ui;
    dataStreamer *streamHandler;

signals:
private slots:
};

#endif // STREAMINGDISPLAY_H
