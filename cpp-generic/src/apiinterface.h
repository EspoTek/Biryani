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
    //Other Functions
    void testAction();
    void testAction_2();
    std::vector<double>* getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
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
    int num_channels_excluding_ref;
};

#endif // APIINTERFACE_H
