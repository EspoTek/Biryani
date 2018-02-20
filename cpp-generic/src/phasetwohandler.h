#ifndef PHASETWOHANDLER_H
#define PHASETWOHANDLER_H

#include <chrono>
#include <thread>
#include <mutex>
#include <vector>

class usbInterface;

class phaseTwoHandler
{
public:
    phaseTwoHandler();
    ~phaseTwoHandler();
    void deleteThread();
    int enterPhaseTwo(int phase_2_packet_length, int num_channels_excluding_ref, usbInterface *interface);
    double getAverageLatency();
    std::vector<double>* getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
private:
    usbInterface *phase2_interface = NULL;
    std::thread *worker = NULL;
};

#endif // PHASETWOHANDLER_H
