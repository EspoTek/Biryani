// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BIRYANI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BIRYANI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#include <vector>

#ifdef BIRYANI_EXPORTS
#define BIRYANI_API __declspec(dllexport)
#else
#define BIRYANI_API __declspec(dllimport)
#endif

// This class is exported from the biryani.dll
class BIRYANI_API Cbiryani {
public:
	Cbiryani(void);
	// TODO: add your methods here.
};

extern BIRYANI_API int nbiryani;

//Other
BIRYANI_API double fnbiryani(double number_in);
BIRYANI_API int biryani_enable_debugging_console();
BIRYANI_API int biryani_init();
BIRYANI_API int biryani_reset();

//Copied from API object.
	//Generic Getters
	BIRYANI_API int biryani_getNumChannelsExcludingRef();
	BIRYANI_API int biryani_getPacketInterval();
	BIRYANI_API double biryani_getAverageLatency_ms();
	//Generic Setters
	BIRYANI_API void biryani_setDebugLevel(int new_debug_level_in);
	BIRYANI_API void biryani_setPacketInterval(int new_packet_interval);
	//Synamps Specific Functions
	BIRYANI_API int biryani_loadFile(char *fname);
	BIRYANI_API int biryani_initialiseSynamps2Device();
	BIRYANI_API int biryani_stopStream();
	BIRYANI_API int biryani_startStream();
	BIRYANI_API	int measureSampleRate();
	//Other Functions
	BIRYANI_API std::vector<double>* getData_singleChannel_recent(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
	BIRYANI_API std::vector<double>* getData_singleChannel_sinceLastCall(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length);
	BIRYANI_API std::vector<double>** getData_allChannels_recent(double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);

