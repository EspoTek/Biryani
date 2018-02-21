#include "phasetwohandler.h"
#include "phasetwothreading.h"
#include "usbinterface.h"
#include "subpacketdecoder.h"


phaseTwoHandler::phaseTwoHandler()
{
}

phaseTwoHandler::~phaseTwoHandler()
{
    if(worker != NULL){
        write_kms(true);
        worker->join();
        delete worker;
    }
    if(phase2_interface != NULL) delete phase2_interface;
    phaseTwoThreadData.count = 0;
    phaseTwoThreadData.total_latency = 0;
    phaseTwoThreadData.decoder_sp = NULL;
    write_kms(false);
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
    write_kms(false);
    worker = new std::thread(workerFunction);
    return 0;
}


void workerFunction(){
    int packetCount = 1;
    int bytes_transferred;
    int packet_length = phaseTwoThreadData.packet_length;
    unsigned char *buffer = (unsigned char *) malloc(packet_length);

    //Set up the necessary structures for subpacket decoding.
    printf_debugging("Setting up subPacketDecoder with %d channels (excluding ref)\n", phaseTwoThreadData.num_channels_excluding_ref);
    subPacketDecoder *decoder_sp = new subPacketDecoder(phaseTwoThreadData.num_channels_excluding_ref);
    printf_debugging("Successfully set up subPacketDecoder with a subPacket size of %d\n", decoder_sp->numBytesPerSubpacket());

    phaseTwoThreadData.decoder_sp = decoder_sp;
    bool subPacketInitialiseSuccess = false;
    int packetStartOffset = 0;
    unsigned char *subPacketPointer = NULL;
    bool firstPacketDecoded = false;
    int numSubPacketsToDecode;
    int numLeftoverBytes;
    int num_to_test = 8;

    while(!read_kms()){
        std::chrono::steady_clock::time_point tic = std::chrono::steady_clock::now();
        //Fetch data
        phaseTwoThreadData.interface->transfer_bulk(true, 0x06, buffer, buffer, packet_length, &bytes_transferred);
        if(bytes_transferred == packet_length){
            printf_periodic("Packet decoding begins at position %d\n", packetStartOffset);
            //Calculate time delay
            std::chrono::steady_clock::time_point toc = std::chrono::steady_clock::now();
            std::chrono::steady_clock::duration duration = toc - tic;
            std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(toc - tic);
            printf_periodic("Packet #%d received after a %fms delay.  %d bytes transferred\n", packetCount, time_span.count() * 1000, bytes_transferred);

            write_latency_values(time_span.count() * 1000, &phaseTwoThreadData);

            //Initialise subpacket decoding
            if(!firstPacketDecoded){
                //This code runs the first time we enter the stream.
                //It's highly probable that we've started in a random position mid-subpacket.
                //This code should find where, specifically.
                subPacketPointer = &buffer[0];

                printf_debugging("Seeking start of stream.\n");
                for(int i=0; i<(packet_length - (decoder_sp->numBytesPerSubpacket() * num_to_test)); i++){
                    if(decoder_sp->isStartofStream(subPacketPointer+packetStartOffset, num_to_test)){
                        //num_to_test objects in a row that look like subpackets should indicate a subpacket stream.  If it looks like a duck, and quacks like a duck...
                        subPacketInitialiseSuccess = true;
                        printf_debugging("Start of stream found with offset %d\n", packetStartOffset);
                        break;
                    } else {
                        printf_verbose("subPacket not found with offset %d\n", packetStartOffset);
                        packetStartOffset++;
                    }
                }
            }

            if(subPacketInitialiseSuccess==false){
                fprintf(stderr, "Count not find start of stream.  Did you try to start the stream twice?  Killing thread...\n");
                return;
            }

            //Pointer to the first subPacket in the buffer
            subPacketPointer = buffer + packetStartOffset;

            //subPacketPointer should be pointing to the first whole subpacket in the stream.
            //But is it?
            if(!decoder_sp->isValidSubPacketStream(subPacketPointer, num_to_test)){
                fprintf(stderr, "ERROR: subPacketPointer (offset = %d) is not pointing to a subPacket!", packetStartOffset);
                //return;
            }

            //But what about the partial subpacket that could be before it?  interPacketbuffer saves the day.
            if(firstPacketDecoded){
                decoder_sp->fill_interPacket_back(buffer, packetStartOffset);
                //If not the first packet, interPacketBuffer should now contain a whole subpacket.
                decoder_sp->decodeInterPacket();
            }

            //How many subpackets do we need to decode,?
            numSubPacketsToDecode = (packet_length - packetStartOffset)/(decoder_sp->numBytesPerSubpacket());
            //What's left over?
            numLeftoverBytes = (packet_length - packetStartOffset)%(decoder_sp->numBytesPerSubpacket());

            //Actually decode all of the subPackets
            for (int i=0;i<numSubPacketsToDecode;i++){
                printf_verbose("Accessing bytes #%lld to %lld of buffer\n", &subPacketPointer[i*decoder_sp->numBytesPerSubpacket()] - buffer, &subPacketPointer[i*decoder_sp->numBytesPerSubpacket()] - buffer + decoder_sp->numBytesPerSubpacket());
                decoder_sp->decodeSubPacket(&subPacketPointer[i*decoder_sp->numBytesPerSubpacket()]);
            }

            //But there's that leftover data at the end.
            decoder_sp->fill_interPacket_front(&buffer[packet_length-numLeftoverBytes], numLeftoverBytes);

            //And don't forget to update the packet start offset for the next packet!
            packetStartOffset = decoder_sp->numBytesPerSubpacket() - numLeftoverBytes;

            firstPacketDecoded = true;

            //Increment the packet counter, for debugging
            packetCount++;
        }
    }
    //Code to run when exiting thread.
    phaseTwoThreadData.count = 0;
    phaseTwoThreadData.total_latency = 0;
    phaseTwoThreadData.decoder_sp = NULL;    
    delete decoder_sp;
    printf_debugging("Worker thread terminated correctly");
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

void write_latency_values(double new_latency_value, threadStruct *thread_data){
    thread_data->latency_storage_mutex.lock();
    thread_data->total_latency += new_latency_value;
    thread_data->count += 1;
    thread_data->latency_storage_mutex.unlock();
}

double read_latency_values(threadStruct *thread_data){
    double retVal;
    thread_data->latency_storage_mutex.lock();
    retVal = thread_data->total_latency / (thread_data->count);
    thread_data->latency_storage_mutex.unlock();

    return retVal;
}

double phaseTwoHandler::getAverageLatency(){
    return read_latency_values(&phaseTwoThreadData);
}


std::vector<double> *phaseTwoHandler::getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int *length){
    if(read_kms()){
        fprintf(stderr, "ERROR: Cannot get data.  System has shut down or is preparing for shutdown...\n");
        return NULL;
    }
    if (phaseTwoThreadData.decoder_sp == NULL){
        fprintf(stderr, "ERROR: No subpacket decoder detected.  Have you started the stream yet?\n");
        return NULL;
    } else {
        return phaseTwoThreadData.decoder_sp->getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length);
    }
}

std::vector<double> *phaseTwoHandler::getAllDownSampledChannelDataSinceLastCall_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length){
    if(read_kms()){
        fprintf(stderr, "ERROR: Cannot get data.  System has shut down or is preparing for shutdown...\n");
        return NULL;
    }
    if (phaseTwoThreadData.decoder_sp == NULL){
        fprintf(stderr, "ERROR: No subpacket decoder detected.  Have you started the stream yet?\n");
        return NULL;
    } else {
        return phaseTwoThreadData.decoder_sp->getAllDownSampledChannelDataSinceLastCall_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_max_seconds, length);
    }
}

int phaseTwoHandler::measureSampleRate(){
    return phaseTwoThreadData.decoder_sp->measureSampleRate();
}

