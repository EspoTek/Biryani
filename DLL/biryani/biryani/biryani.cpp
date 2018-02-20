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
