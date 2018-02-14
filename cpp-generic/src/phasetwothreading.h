#ifndef PHASETWOTHREADING_H
#define PHASETWOTHREADING_H

class usbInterface;
class subPacketDecoder;

typedef struct threadStruct{
    int packet_length;
    int num_channels_excluding_ref;
    bool kms = false;
    std::mutex kms_mutex;
    std::mutex latency_storage_mutex;
    usbInterface *interface;
    subPacketDecoder *decoder_sp = NULL;
    double total_latency = 0;
    double count = 0;
} threadStruct;

threadStruct phaseTwoThreadData;
//When kms = true, worker thread will kill itself when safe to do so

void workerFunction();
bool read_kms();
void write_kms(bool new_value);
void write_latency_values(double new_latency_value, threadStruct *thread_data);
double read_latency_values(threadStruct *thread_data);

#endif // PHASETWOTHREADING_H
