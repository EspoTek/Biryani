#ifndef PHASETWOTHREADING_H
#define PHASETWOTHREADING_H

void workerFunction();
class usbInterface;

typedef struct threadStruct{
    int packet_length;
    int num_channels_excluding_ref;
    bool kms = false;
    std::mutex kms_mutex;
    usbInterface *interface;
} threadStruct;

threadStruct phaseTwoThreadData;
//When kms = true, worker thread will kill itself when safe to do so
bool read_kms();
void write_kms(bool new_value);


#endif // PHASETWOTHREADING_H
