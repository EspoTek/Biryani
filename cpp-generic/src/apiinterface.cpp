#include "apiinterface.h"
#include "configurationfilehandler.h"
#include "usbinterface.h"
#include "phasezerohandler.h"
#include "phaseonehandler.h"
#include "phasetwohandler.h"

int debug_level = 0;

//apiInterface() is the constructor function for the API object.
//It just initialises and does what constructors do.
//Returns void.
apiInterface::apiInterface()
{
    fileHandler = new configurationFileHandler(&phase1_raw, &phase2_length, &phase3_raw, &num_channels_excluding_ref);
    usbHandler = new usbInterface(SYNAMPS2_MAIN_VID, SYNAMPS2_MAIN_PID);
    p0handler = new phaseZeroHandler();
    p1handler = new phaseOneHandler(&phase1_raw);
    p2handler = new phaseTwoHandler();
    p3handler = new phaseOneHandler(&phase3_raw);

    //Remove buffering from printf() statements.  Slower, but at least they will be in the right order.
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

//Destructor!
apiInterface::~apiInterface(){
    delete p3handler;
    delete p2handler;
    delete p1handler;
    delete p0handler;
    delete usbHandler;
    delete fileHandler;
}

//setDebugLevel() sets the debug level variable.
//This variable controls what debugging messages are displayed.
//If it's greater than or equal to 400, it will display all messages.
//If it's between 300 and 400, it will display some debugging messages.
//If it's less than 300, it will display only some messages.

//Note that this same variable also controls libusb's debug level.
//To set the libusb debug level, you must call this function before starting the stream.
//If you do not want to see libusb debugging messages, call this function after starting the stream.

//Default debug level is 0.
//Returns void.
void apiInterface::setDebugLevel(int new_debug_level_in){
    debug_level = new_debug_level_in;
    printf("Debug Level changed to %d\n", debug_level);
}

//loadFile() loads a .bcf file and decodes it.
//fname should be a string that contains either a full or relative path to the .bcf file.  (Full path recommended).
int apiInterface::loadFile(char *fname){
    return fileHandler->loadFile(fname);
}

//The Synamps2 device actually enumerates as two separate USB devices: 0B6E/0005 and 0B6E/0006.
//When you first turn it on, it will enumerate as 0B6E/0005 and you cannot stream any data from it.
//initialiseSynamps2Device() sends the necessary initialisation data to it, so that it will enumerate as 0B6E/0006.
//Once this initialisation step is complete, the stream can be started.
//Initialisation only needs to be performed once each time the device is switched on.
//This function can be called before or after loadFile().
//Returns a negative value and gives an error message on failure.
int apiInterface::initialiseSynamps2Device(){
    int error = 0;

    //Check to see if either a 0B6E/0005 or 0B6E/0006 is connected.
    int amplifierStatus = p0handler->checkIfAlreadyInitialised();

    //If we have a 0B6E/0005, initialise it.
    //If we have a 0B6E/0006, do nothing since it's already initialised.
    //If we have neither, spit out an error since it implies that either the driver is munted or the device is not connected.
    switch(amplifierStatus){
        case -1:
            fprintf(stderr, "Synamps2 device not detected!\n");
            return -420;
        case 0:
            printf_debugging("Synamps2 device connected but not ready.  Connecting...\n");
            error = p0handler->createPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->createInitPattern(): 0x%08x", error);
                return error;
            }
            error = p0handler->sendPattern();
            if(error){
                fprintf(stderr, "ERROR in p0handler->sendInitPattern(): 0x%08x", error);
                return error;
            }
            return 0;
        case 1:
            printf("Synamps2 device already connected and ready.  Returning without initialising.\n");
            return 420;
        default:
            fprintf(stderr, "ERROR in initialiseSynamps2Device(), amplifer status invalid");
            return -69;
    }
}

//startStream() performs two functions.
//First, it configures the device using the data from the .bcf file (phase 1).
//Then, it initialises and starts the stream handling thread as well as its associated objects.
//Returns a non-zero value on failure.
int apiInterface::startStream(){
    //We don't want to start it twice.
    if(streamStarted){
        fprintf(stderr, "Stream already started!\n");
        return -69420;
    }

    //Configure the device using the settings loaded from the .bcf file.
    int unconfigured = configureSynamps2Device();

    //If it configured correctly, enter phase 2.  Otherwise, abort with error.  (No cleanup?)
    if(unconfigured){
        return 69420;
    }
    int phaseTwoFailed = p2handler->enterPhaseTwo(phase2_length, num_channels_excluding_ref, p1handler->getInterface());

    //If Phase two has begun successfully, set the streamStarted variable to true and return success.  Otherwise, abort.  (No cleanup?)
    if(phaseTwoFailed){
        return  phaseTwoFailed;
    }
    streamStarted = true;
    return 0;
}

//stopStream() tells the device that no more streaming will take place, and then kills the stream handling thread.
//Returns a non-zero value on failure.
int apiInterface::stopStream(){
    int error;

    //We don't want to stop a thread that's not already started.
    if(!streamStarted){
        fprintf(stderr, "Cannot stop the stream since it has not been started yet!\n");
        return -69420;
    }

    //Send phase 3 data to device.
    error = p3handler->createPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p3handler->createPattern().  Aborting...\n", error);
        return error;
    }
    error = p3handler->sendPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p3handler->sendPattern().  Aborting...\n", error);
        return error+1000;
    }

    //Safely kill the thread.
    p2handler->deleteThread();

    streamStarted = false;
    return 0;
}

//getData_singleChannel_recent() returns a vector of samples on success.
//The first sample contains the value of the stream (delay_seconds + timeWindow_seconds) seconds ago.
//The last samples contains the value of the stream delay_seconds seconds ago.
//The samples in between are evenly spaced, temporally, with interval of (1/sampleRate_Hz) seconds per sample.
//Note that the first channel has an index of 0.  Channel 1 is the second channel.
//Returns NULL on error, otherwise returns the vector.
//Please note that the pointer is freed upon the next call to getData_xxxx_xxxx()
std::vector<double>* apiInterface::getData_singleChannel_recent(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length){
    if(channel > getNumChannelsExcludingRef()){
        fprintf(stderr, "ERROR: Attempted to access nonexistant channel.\n");
        return NULL;
    }
    return p2handler->getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length);
}

//See definition of getData_singleChannel_recent().
//This function is more-or-less identical, but it will only return data that has been streamed from the device received since the last time this function was called.
//delay_seconds dictates the delay, as per getData_singleChannel_sinceLastCall(), while timeWindow_max_seconds dictates the maximum size of the time window you want to look at.
//For example, let's assume you called this function once at t=10s with delay_seconds = 0.1 and timeWindow_max_seconds=5, and then again at and t=20s with delay_seconds = 0 and timeWindow_max_seconds=999999999999999.
//The first call would return the data corresponding to the t=4.9s to t=9.9s, and the second call would contain t=9.9s to t=19.9s.
//Note that the first channel has an index of 0.  Channel 1 is the second channel.
//It would not contain anything earlier than 9.9s since that was returned by the last call, and it would not contain anything later than 19.9 since there is a 0.1s delay
std::vector<double>* apiInterface::getData_singleChannel_sinceLastCall(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length){
    if(channel > getNumChannelsExcludingRef()){
        fprintf(stderr, "ERROR: Attempted to access nonexistant channel.\n");
        return NULL;
    }
    return p2handler->getAllDownSampledChannelDataSinceLastCall_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_max_seconds, length);
}

//See definition of getData_singleChannel_recent().
//This functions much the same but returns all channels.
//Some software trickery is used to ensure that there is no phase shift between channels (well, no phase shift introduced by the PC).
//You can safely assume that sample n in channel i was taken at the same time as sample n in channel j.
std::vector<double>** apiInterface::getData_allChannels_recent(double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length){
    if(getNumChannelsExcludingRef() == 0){
        fprintf(stderr, "ERROR: It appears as though there are zero channels!\n");
        return NULL;
    }
    return p2handler->getData_allChannels_recent(sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length);
}


//getNumChannelsExcludingRef() returns the number of channels (excluding reference) from the .bcf file.
//Returns -1 if no file is loaded
int apiInterface::getNumChannelsExcludingRef(){
    return num_channels_excluding_ref;
}

//Once the stream has started, the stream handling thread enters a loop of:
//1: Read in (packetInterval * 512) bytes of packet data from the device.
//2: Extract the samples from the packet data and store it in an internal API structure.
//3: Go to step 1.
//Note the variable packetInterval in step 1 of the loop.  This is one of the two variables that controls latency; the other being the number of channels.
//Latency for a given number of channels scales linearly with packetInterval.
//If you have 17ms of latency with 4 channels and packetInterval=8, you'll have 34ms of latency with 4 channels and packetInterval=16.
//It tells you nothing about latency when the channel count is not 4, though.
//On success, getPacketInterval() returns the packetInterval variable, as described above.
//On failure, it returns -1.
int apiInterface::getPacketInterval(){
    printf_verbose("Phase2_length = %d\n", phase2_length);
    if(phase2_length % 512){
        fprintf(stderr, "PHASE_2_DATA is not a multiple of 512.  Check your .bcf file for corruption.\n");
        return -1;
    }

    if(phase2_length == 0){
        fprintf(stderr, "PHASE_2_DATA = 0.  Have you loaded the .bcf yet?\n");
    }

    return phase2_length/512;
}

//setPacketInterval() sets the packetInterval variable, as described in getPacketInterval().
//Returns void.
void apiInterface::setPacketInterval(int new_packet_interval){
    phase2_length = new_packet_interval * 512;
}

//getAverageLatency_ms() gets the average time in ms between data buffer updates.
//A value of 20ms means that you can get new data using getDownSampledChannelData_double() or getAllDownSampledChannelDataSinceLastCall_double() every 20ms, on average.
//If you're not satisfied with the current latency you're getting, you can reduce it using setPacketInterval().  Latency scales linearly with the packet interval.
//Just keep in mind that this won't fix a bottleneck caused by inefficient processing of the data.  :)
double apiInterface::getAverageLatency_ms(){
    return p2handler->getAverageLatency();
}

//measureSampleRate() takes a measurement of the sampling of the amplifier, in Hz.
//Please note that it will take about 2 seconds to perform this measurement.
//It should always return approximately 10,000.  If not, something is wrong.
int apiInterface::measureSampleRate(){
    if(!streamStarted){
        fprintf(stderr, "Cannot measure the sample rate until the stream has started!");
        return -1;
    }
    return p2handler->measureSampleRate();
}

//configureSynamps2Device() is an internal function called by startStream().
//Basically does all the phase 1 stuff but doesn't enter phase 2.
int apiInterface::configureSynamps2Device(){
    int error;

    error = p1handler->createPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p1handler->createPattern().  Aborting...\n", error);
        return error;
    }
    error = p1handler->sendPattern();
    if(error < 0){
        fprintf(stderr, "ERROR %d in p1handler->sendPattern().  Aborting...\n", error);
        return error+1000;
    }
    return 0;
}
