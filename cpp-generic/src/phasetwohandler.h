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
    int enterPhaseTwo(int phase_2_packet_length, int num_channels_excluding_ref, double sample_rate_in, usbInterface *interface);
    double getAverageLatency();
    int measureSampleRate();
    std::vector<double>* getData_singleChannel_recent(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
    std::vector<double>* getData_singleChannel_sinceLastCall(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length);
    std::vector<double>** getData_allChannels_recent(double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
    std::vector<double>** getData_allChannels_sinceLastCall(double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length);
private:
    usbInterface *phase2_interface = NULL;
    std::thread *worker = NULL;
};

#endif // PHASETWOHANDLER_H
