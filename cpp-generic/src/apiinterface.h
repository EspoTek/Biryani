#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include "api_defines.h"

class configurationFileHandler;
class phaseZeroHandler;
class phaseOneHandler;
class phaseTwoHandler;
class usbInterface;

//apiInterface is the main class of the Biryani 7 API.
//It takes all external calls and routes them to the appropriate objects.

class apiInterface
{
public:
    apiInterface();
    ~apiInterface();
    //Generic Getters
    int getNumChannelsExcludingRef();
    int getPacketInterval();
    double getAverageLatency_ms();
    //Generic Setters
    void setDebugLevel(int new_debug_level_in);
    void setPacketInterval(int new_packet_interval);
    //Synamps Specific Functions
    int loadFile(char *fname);
    int initialiseSynamps2Device();
    int stopStream();
    int startStream();
    int measureSampleRate();
    //Other Functions
    std::vector<double>* getData_singleChannel_recent(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
    std::vector<double>* getData_singleChannel_sinceLastCall(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length);
    std::vector<double>** getData_allChannels_recent(double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
private:
    //Hidden fucntions
    int configureSynamps2Device();

    //Internal object pointers
    configurationFileHandler *fileHandler;
    phaseZeroHandler *p0handler;
    phaseOneHandler *p1handler;
    phaseTwoHandler *p2handler;
    phaseOneHandler *p3handler;
    usbInterface *usbHandler;

    //"Global" (within API) variables
    std::vector<rawPacket> phase1_raw;
    int phase2_length;
    std::vector<rawPacket> phase3_raw;
    int num_channels_excluding_ref = -1;
    bool streamStarted = false;
};

#endif // APIINTERFACE_H
