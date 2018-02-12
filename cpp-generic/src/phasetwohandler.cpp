#include "phasetwohandler.h"
#include "phasetwothreading.h"
#include "usbinterface.h"


phaseTwoHandler::phaseTwoHandler()
{
}

void phaseTwoHandler::deleteThread(){
    write_kms(true);
}

int phaseTwoHandler::enterPhaseTwo(int phase2_packet_length_in, int num_channels_excluding_ref, usbInterface *interface){
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
    phaseTwoThreadData.num_channels_excluding_ref =num_channels_excluding_ref;
    worker = new std::thread(workerFunction);
    return 0;
}


void workerFunction(){
    int packetCount = 1;
    int bytes_transferred;
    unsigned char *buffer = (unsigned char *) malloc(phaseTwoThreadData.packet_length);

    while(!read_kms()){
        std::chrono::steady_clock::time_point tic = std::chrono::steady_clock::now();
        //Fetch data
        phaseTwoThreadData.interface->transfer_bulk(true, 0x06, buffer, buffer, phaseTwoThreadData.packet_length, &bytes_transferred);
        if(bytes_transferred){
            //Calculate time delay
            std::chrono::steady_clock::time_point toc = std::chrono::steady_clock::now();
            std::chrono::steady_clock::duration duration = toc - tic;
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(toc - tic);
            printf("Packet #%d received after a %fms delay.  %d bytes transferred\n", packetCount, time_span * 1000, bytes_transferred);

            //Add the data
            packetCount++;
        }
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
