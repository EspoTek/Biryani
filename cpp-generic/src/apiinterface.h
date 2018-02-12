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
    void testAction();
    void testAction_2();
    void setDebugLevel(int new_debug_level_in);
    int loadFile(char *fname);
    int initialiseSynamps2Device();
    int configureSynamps2Device();
    int stopStream();
    int startStream();
    std::vector<double>* getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
private:
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
