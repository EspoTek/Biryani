// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BIRYANI_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BIRYANI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
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

BIRYANI_API double fnbiryani(double number_in);
BIRYANI_API int biryani_init();
BIRYANI_API int biryani_enable_debugging_console();
