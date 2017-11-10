#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"
#include "QDebug"

//Please note that, in order to understand this code, you'll need some understanding of the USB protocol, and I guess packet sniffing in general.
//I recommend Jan Axelson's "USB Complete" if you want to take things seriously, or "USB in a nutshell" for a quick intro.
//http://janaxelson.com/usbc.htm
//http://www.beyondlogic.org/usbnutshell/usb1.shtml
//I have a copy of USB Complete if you want to borrow it.
//Don't bother reading the standard unless you're already well aquainted with USB; it's possibly one of the least elegant documents ever written.

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Set up the interface
    ui->setupUi(this);
    ui->spinBox->setMaximum(NUM_WIRESHARK_PACKETS_stage2);
    connect(ui->inspectButton, SIGNAL(clicked(bool)), this, SLOT(buttonPressed(void)));
    connect(ui->extractInitButton, SIGNAL(clicked(bool)), this, SLOT(createInitPattern(void)));
    connect(ui->sendInitButton, SIGNAL(clicked(bool)), this, SLOT(initialiseAmplifier(void)));
    connect(ui->extractPreinitButton, SIGNAL(clicked(bool)), this, SLOT(createPreinitPattern(void)));
    connect(ui->sendPreinitButton, SIGNAL(clicked(bool)), this, SLOT(preinitialiseAmplifier(void)));
    connect(ui->startDataStreamButton, SIGNAL(clicked(bool)), this, SLOT(launchStreamingDisplay(void)));
    setWindowTitle("BIRYANI7");
    libusb_interface_main = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    libusb_interface_preinit = new usbInterface(SYNAMPS2_PREINIT_VID, SYNAMPS2_PREINIT_PID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//This function handles the printing of the packet info when you click the "inspect" button.
//It's mainly just for debugging purposes.
void MainWindow::packetInspect(int packetNumber){
    unsigned int packetLength = wireshark_packet_lengths_stage2[packetNumber];
    const unsigned char *packet = wireshark_packets_stage2[packetNumber];
    unsigned char pseudoHeaderLength = packet[0];
    unsigned char direction = packet[21] & 0x80;
    char direction_string[4] = "NUL";
    strcpy(direction_string, direction ? "IN" : "OUT");

    unsigned char endpoint = packet[21] & 0x0f;
    unsigned char transfer_type = packet[22];

    char transfer_type_string[10];
    switch (transfer_type){
        case 0x02:
            strcpy(transfer_type_string, "CONTROL");
            break;
        case 0x03:
            strcpy(transfer_type_string, "BULK");
            break;
        default:
            strcpy(transfer_type_string, "UNKNOWN");
    }

    //For control packets.  Note that usbpcap splits the whole control transfer into three packets;
    //one for each of the setup/data/status stages
    unsigned char control_stage = 0;
    unsigned char bmRequestType = 0;
    unsigned char bRequest = 0;
    unsigned short wValue = 0;
    unsigned short wIndex = 0;
    unsigned short wLength = 0;

    char control_stage_string[10] = "INVALID";

    //0x02 is control transfer
    if(transfer_type==0x02){
        control_stage = packet[27];
        switch(control_stage){
            case 0x00:
                strcpy(control_stage_string, "SETUP");
                break;
            case 0x01:
                strcpy(control_stage_string, "DATA");
                break;
            case 0x02:
                strcpy(control_stage_string, "STATUS");
                break;
            default:
                strcpy(control_stage_string, "UNKNOWN");
        }
    }

    //These fields are only applicable to control transfers, and only visible during the setup stage
    if(transfer_type==0x02 && control_stage == 0){
        bmRequestType = packet[28];
        bRequest = packet[29];
        wValue = packet[30] + 256 * packet[31];
        wIndex = packet[32] + 256 * packet[33];
        wLength = packet[34] + 256 * packet[35];
    }

    //Output all of the values
    qDebug() << "\n\n\nPacket Number:" << packetNumber;
    qDebug() << "Packet Length:" << packetLength;
    qDebug() << "Pseudoheader Length:" << pseudoHeaderLength;
    qDebug() << "Direction:" << direction_string;
    qDebug() << "Endpoint:" << endpoint;
    qDebug() << "Transfer Type:" << transfer_type_string;
    if(transfer_type==0x02){
        qDebug() << "Control Stage:" << control_stage_string;
    }
    if(transfer_type==0x02 && control_stage == 0){
        qDebug("bmRequestType: %02x", bmRequestType);
        qDebug("bRequest: %02x", bRequest);
        qDebug() << "wValue:" << wValue;
        qDebug() << "wIndex:" << wIndex;
        qDebug() << "wLength:" << wLength;
    }

    return;
}

//This function creates a chain of packets that, when sent one after another, will initialise the Synamps2 device to a given configuration.
void MainWindow::createInitPattern(void){
    qDebug() << "createInitPattern";

    //We don't want to run this function twice!
    if(initPattern.size()){
        qDebug("initPattern already contains %d elements!", initPattern.size());
        return;
    }
    initTransfer *currentTransfer = NULL;
    unsigned int packetNumber = 0;
    bool enteredDataStage = false;
    unsigned int packetLength;
    const unsigned char *packet;
    unsigned char transfer_type;
    unsigned char control_stage;
    unsigned char pseudoHeaderLength;

    //The function needs to iterate over the packets in the init stage only, stopping once we enter the data transfer stage.
    //Most of the offsets in the code below (e.g. transfer type being at position [22]) come from the structure of usbpcap's packets.
    //Open the .pcap file in Wireshark if you want to understand exactly what it is reading.
    while(!enteredDataStage){
        packetLength = wireshark_packet_lengths_stage2[packetNumber];
        packet = wireshark_packets_stage2[packetNumber];
        pseudoHeaderLength = packet[0];
        transfer_type = packet[22];

        //Just in case userdefinedproperties.h is configured incorrectly
        if(packetNumber==NUM_WIRESHARK_PACKETS_stage2){
            qFatal("I've gone through the whole packet capture data but didn't find a single data stage packet...\nDid you configure userdefinedproperties.h correctly?");
        }

        //Do not set the control_stage field for a BULK transfer.
        if(transfer_type==0x02){
            control_stage = packet[27];
        }

        //Only the pointer to the data needs to be updated in the data stage.
        if((transfer_type == 0x02) && (control_stage == 0x01)){ //CONTROL-DATA
            currentTransfer->data = &packet[pseudoHeaderLength];
            goto createInitPattern_end_loop;
        }

        //Because I'm lazy, I'll assume that all status stages return "success".
        if((transfer_type == 0x02) && (control_stage == 0x02)){ //CONTROL-STATUS
            goto createInitPattern_end_loop;
        }

        //A Bulk or (setup stage) control packet indicates a new USB transfer.
        currentTransfer = new initTransfer(libusb_interface_main);
        initPattern.push_back(currentTransfer);

        //These fields only need to be set once per transfer
        currentTransfer->direction_is_in = (packet[21] & 0x80) ? 0x01 : 0x00;
        currentTransfer->endpoint = packet[21] & 0x0f;
        currentTransfer->transfer_type = transfer_type;

        //Bulk packets have a slightly different structure to control packets, so if() statements are needed.
        if(transfer_type == 0x03){
            currentTransfer->data = &packet[pseudoHeaderLength];
            currentTransfer->data_length = packet[23] + 0x100*packet[24] + 0x10000*packet[25] + 0x1000000*packet[26];
            //These values will be unused but it makes debugging clearer
            currentTransfer->bmRequestType = 69;
            currentTransfer->bRequest = 69;
            currentTransfer->wValue = 69;
            currentTransfer->wIndex = 69;
        }
        //There is an implicit assumption in the following if() statement that the control transfer is in the setup stage.
        //It should always be true because if it was not in the setup stage then it should be caught by one of the gotos above.
        if(transfer_type==0x02){ //CONTROL Only
            currentTransfer->bmRequestType = packet[28];
            currentTransfer->bRequest = packet[29];
            currentTransfer->wValue = packet[30] + 0x100*packet[31];
            currentTransfer->wIndex = packet[32] + 0x100*packet[33];
            currentTransfer->data_length = packet[34] + 0x100*packet[35];
        }

        //Increment counter and check for termination
        createInitPattern_end_loop:
        packetNumber++;
        //I am sufficiently convinced that the below termination condition is good enough to detect when we've entered the data stage.
        if((currentTransfer->direction_is_in) && (currentTransfer->endpoint == 0x06) && (currentTransfer->data_length == LENGTH_DATA_PACKET)){
            //By this stage, we've found our first data packet.  We need to remove it from the init pattern.
            initPattern.pop_back();
            enteredDataStage = true;
        }       
    }
    qDebug() << initPattern.size();
}

//The Synamps2 device actually enumerates as two separate USB devices: 0B6E/0005 and 0B6E/0006.
//When you first turn it on, it will enumerate as 0B6E/0005.
//You then need to send some configuration or firmware (?) data to the device using vendor command 0xa0.
//After this is done, it will enumerate as 0B6E/0006 and be ready for initialisation.
//This function creates a chain of packets that sends the aforementioned configuration/firmware data.
void MainWindow::createPreinitPattern(void){
    qDebug() << "createPreinitPattern";

    //We don't want to run this function twice!
    if(preinitPattern.size()){
        qDebug("initPattern already contains %d elements!", preinitPattern.size());
        return;
    }
    initTransfer *currentTransfer = NULL;
    unsigned int packetNumber = 0;
    bool enteredDataStage = false;
    unsigned int packetLength;
    const unsigned char *packet;
    unsigned char transfer_type;
    unsigned char control_stage;
    unsigned char pseudoHeaderLength;
    bool current_packet_is_0xa0 = false;

    //This function needs to iterate over all packets.
    //Those that are not control-vendor command 0xa0 should be ignored.
    for(packetNumber = 0; packetNumber<NUM_WIRESHARK_PACKETS_stage1; packetNumber++){
        packetLength = wireshark_packet_lengths_stage1[packetNumber];
        packet = wireshark_packets_stage1[packetNumber];
        pseudoHeaderLength = packet[0];
        transfer_type = packet[22];

        if(transfer_type==0x03){
            //We're not interested in bulk packets because they're not vendor 0xa0.
            continue;
        }

        control_stage = packet[27];

        //Only the pointer to the data needs to be updated in the data stage.
        if((transfer_type == 0x02) && (control_stage == 0x01) && current_packet_is_0xa0){ //CONTROL-DATA
            currentTransfer->data = &packet[pseudoHeaderLength];
            continue;
        }

        //Because I'm lazy, I'll assume that all status stages return "success".
        if((transfer_type == 0x02) && (control_stage == 0x02)){ //CONTROL-STATUS
            current_packet_is_0xa0 = false;
            continue;
        }

        //If neither of the above two if()s triggered, we're looking at a setup stage control packet.
        //Now all that's left to check is that the command is vendor-0xa0
        //First condition checks bmRequestType to ensure it's vendor; second checks that it's of type 0xa0
        if(((packet[28] & 0x60) != 0x40) || (packet[29] != 0xa0)){
            if((packet[28] & 0x60) != 0x40) qDebug("Packet %d is not of type vendor!", packetNumber);
            if(packet[29] != 0xa0) qDebug("Packet %d does not have bRequest of 0xa0\n", packetNumber);
            continue;
        }

        //If it's reached this stage, it's the setup stage for a control transfer of vendor type 0xa0.
        //Lets allocate it's packet!
        currentTransfer = new initTransfer(libusb_interface_preinit);
        preinitPattern.push_back(currentTransfer);
        current_packet_is_0xa0 = true;

        //These fields only need to be set once per transfer, during the setup stage
        currentTransfer->direction_is_in = (packet[21] & 0x80) ? 0x01 : 0x00;
        currentTransfer->endpoint = packet[21] & 0x0f;
        currentTransfer->transfer_type = transfer_type;
        currentTransfer->bmRequestType = packet[28];
        currentTransfer->bRequest = packet[29];
        currentTransfer->wValue = packet[30] + 0x100*packet[31];
        currentTransfer->wIndex = packet[32] + 0x100*packet[33];
        currentTransfer->data_length = packet[34] + 0x100*packet[35];
    }
    qDebug() << preinitPattern.size();
}

//Simple slot function
void MainWindow::preinitialiseAmplifier(void){
    sendPacketStream(libusb_interface_preinit, &preinitPattern, 0);
}

//Simple slot function
void MainWindow::initialiseAmplifier(void){
    sendPacketStream(libusb_interface_main, &initPattern, 0);
}

//Simple slot function
void MainWindow::buttonPressed(void){
    packetInspect(ui->spinBox->value());
}

//This function handles the actual sending of the series of transfers over the USB interface.
//Interface refers to the BIRYANI7 usbInterface object, bInterface is the "interface" field from the USB protocol.
void MainWindow::sendPacketStream(usbInterface *interface, std::vector<initTransfer*> *pattern, int bInterface){
    int error;

    //Setup the USB device!
    if(interface->setup(bInterface)){
        //Error printing is handled internally by usbInterface::setup().
        return;
    }

    //Actually send the packets!
    for(int i=0; i<pattern->size(); i++){
        error = pattern->at(i)->transmit();
        if(error)
        {
            qDebug("Error sending transfer #%d.  Error code: 0x%08x; %s", i, error, libusb_error_name(error));
        } else qDebug("Transfer #%d sent successfully!", i);
    }
}

//This function launches the second window that displays the data stream.
void MainWindow::launchStreamingDisplay(void){
    qDebug() << "MainWindow::launchStreamingDisplay()";

    //If a window already exists, close it.
    if(sDisplay){
        delete(sDisplay);
    }
    if(libusb_interface_main->setup(0)){
        return;
    };
    sDisplay = new streamingDisplay(this, libusb_interface_main);
    sDisplay->show();
}



