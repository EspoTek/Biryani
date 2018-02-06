#include "phasetwohandler.h"
#include "phasetwothreading.h"
#include "usbinterface.h"


phaseTwoHandler::phaseTwoHandler()
{
}

void phaseTwoHandler::deleteThread(){
    write_kms(true);
}

int phaseTwoHandler::enterPhaseTwo(int phase2_packet_length_in, usbInterface *interface){
    if(interface == NULL){
        fprintf(stderr, "FATAL ERROR: USB Interface is null.  Cannot enter phase 2.  This situation should never occur.\n");
        return -1;
    }
    if(!interface->ready){
        fprintf(stderr, "USB Interface not initialised.  You must successfully complete phase 1 before entering phase 2.  Aborting...\n");
        return -2;
    }

    if(phase2_packet_length_in == 0){
        fprintf(stderr, "Phase 2 (Data) has a packet length of zero.\nAborting phase 2 setup...\nIf you are sure you loaded the .bcf file correctly, check both the .pcap and .bcf files for correct content. \nConsult the documentation regarding what you should see in the .pcap.\nFor the .bcf, open it in Notepad or another text editor and ensure that a line beginning with PHASE_2_DATA, followed by a single positive (non-zero) number is present\n");
        return -3;
    }

    phaseTwoThreadData.packet_length = phase2_packet_length_in;
    phaseTwoThreadData.interface = interface;
    worker = new std::thread(workerFunction);
    return 0;
}


void workerFunction(){
    int packetCount = 1;
    int bytes_transferred;
    unsigned char *buffer = (unsigned char *) malloc(phaseTwoThreadData.packet_length);

    while(!read_kms()){
        //Fetch data
        phaseTwoThreadData.interface->transfer_bulk(true, 0x06, buffer, buffer, phaseTwoThreadData.packet_length, &bytes_transferred);
        if(bytes_transferred){
            printf("Packet #%d received.  %d bytes transferred\n", packetCount, bytes_transferred);
            packetCount++;
        }

        //Wait to avoid saturating thread.
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool read_kms(){
    bool temp_kms_status;
    phaseTwoThreadData.kms_mutex.lock();
    temp_kms_status = phaseTwoThreadData.kms;
    phaseTwoThreadData.kms_mutex.unlock();
    return temp_kms_status;
}

void write_kms(bool new_value){
    phaseTwoThreadData.kms_mutex.lock();
    phaseTwoThreadData.kms = new_value;
    phaseTwoThreadData.kms_mutex.unlock();
}
