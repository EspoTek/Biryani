// biryani.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "biryani.h"
#include "apiinterface.h"

//Macros
#define MACRO_CHECK_API_OBJECT_EXISTS \
if (ptr_api == NULL) { \
	return -12801024; \
}

#define MACRO_VOID_FUNCTION_CHECK_API_OBJECT_EXISTS \
if (ptr_api == NULL) { \
	return; \
}

#define MACRO_VECTOR_RETURN_FUNCTION_CHECK_API_OBJECT_EXISTS \
if (ptr_api == NULL) { \
	return NULL; \
}


// This is an example of an exported variable
BIRYANI_API int nbiryani=0;
apiInterface *ptr_api = NULL;

// This is an example of an exported function.
BIRYANI_API double fnbiryani(double number_in)
{
    return 42 * number_in + 420;
}

BIRYANI_API int biryani_init()
{
	//Check if object already exists
	if (ptr_api != NULL) {
		printf("Biryani API object already exists at address 0x%llx.  Aborting...\n", ptr_api);
		return -1;
	}

	ptr_api = new apiInterface();

	//Ensure it was created properly.
	if (ptr_api == NULL) {
		printf("Failed to create Biryani API object.  Aborting...\n");
		return 1;
	}

	return 0;
}

BIRYANI_API int biryani_reset()
{
	//Check if object already exists
	if (ptr_api == NULL) {
		printf("Biryani API has not been initialised!  Aborting...\n", ptr_api);
		return -1;
	}

	//Delete old API and replace with new one.
	delete ptr_api;
	ptr_api = new apiInterface();

	//Ensure it was created properly.
	if (ptr_api == NULL) {
		printf("Failed to create Biryani API object.  Aborting...\n");
		return 1;
	}

	return 0;
}

BIRYANI_API int biryani_enable_debugging_console()
{
	if (AllocConsole() == 0) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
	return 0;
}

// This is the constructor of a class that has been exported.
// see biryani.h for the class definition
Cbiryani::Cbiryani()
{
    return;
}

BIRYANI_API int biryani_getNumChannelsExcludingRef() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->getNumChannelsExcludingRef();
}
BIRYANI_API int biryani_getPacketInterval() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->getPacketInterval();
}
BIRYANI_API double biryani_getAverageLatency_ms() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->getAverageLatency_ms();
}
BIRYANI_API void biryani_setDebugLevel(int new_debug_level_in) {
	MACRO_VOID_FUNCTION_CHECK_API_OBJECT_EXISTS;
	return ptr_api->setDebugLevel(new_debug_level_in);
}
BIRYANI_API void biryani_setPacketInterval(int new_packet_interval) {
	MACRO_VOID_FUNCTION_CHECK_API_OBJECT_EXISTS;
	return ptr_api->setPacketInterval(new_packet_interval);
}
BIRYANI_API int biryani_loadFile(char *fname) {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->loadFile(fname);
}
BIRYANI_API int biryani_initialiseSynamps2Device() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->initialiseSynamps2Device();
}
BIRYANI_API int biryani_stopStream() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->stopStream();

}
BIRYANI_API int biryani_startStream() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->startStream();
}
BIRYANI_API	int measureSampleRate() {
	MACRO_CHECK_API_OBJECT_EXISTS;
	return ptr_api->measureSampleRate();
}
BIRYANI_API std::vector<double>* biryani_getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length) {
	MACRO_VECTOR_RETURN_FUNCTION_CHECK_API_OBJECT_EXISTS;
	return ptr_api->getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length);
}
BIRYANI_API std::vector<double>* getAllDownSampledChannelDataSinceLastCall_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_max_seconds, int* length) {
	MACRO_VECTOR_RETURN_FUNCTION_CHECK_API_OBJECT_EXISTS;
	return ptr_api->getAllDownSampledChannelDataSinceLastCall_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_max_seconds, length);
}
