#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inittransfer.h"
#include "synamps_config.h"
#include "synamps_preinit.h"
#include "usbinterface.h"
#include "userdefinedproperties.h"
#include "streamingdisplay.h"

#define SYNAMPS2_MAIN_VID 0x0B6E
#define SYNAMPS2_MAIN_PID 0x0006

#define SYNAMPS2_PREINIT_VID 0x0B6E
#define SYNAMPS2_PREINIT_PID 0x0005



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    usbInterface *libusb_interface_main;
    usbInterface *libusb_interface_preinit;

private:
    Ui::MainWindow *ui;
    std::vector<initTransfer*> initPattern;
    std::vector<initTransfer*> preinitPattern;
    void packetInspect(int packetNumber);
    void sendPacketStream(usbInterface *interface, std::vector<initTransfer*> *pattern, int bInterface);
    streamingDisplay *sDisplay = NULL;
public slots:
    void buttonPressed(void);
    void createInitPattern(void);
    void createPreinitPattern(void);
    void initialiseAmplifier(void);
    void preinitialiseAmplifier(void);
    void launchStreamingDisplay(void);
};

#endif // MAINWINDOW_H
