// biryani.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "biryani.h"


// This is an example of an exported variable
BIRYANI_API int nbiryani=0;

// This is an example of an exported function.
BIRYANI_API double fnbiryani(double number_in)
{
    return 42 * number_in + 420;
}

// This is the constructor of a class that has been exported.
// see biryani.h for the class definition
Cbiryani::Cbiryani()
{
    return;
}
