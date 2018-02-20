//MATLAB INCLUDES
#include <matrix.h>  //Do not include if using GNU Octave to compile mex
#include <mex.h>

//Biryani API Include
#include "biryani.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //I/O
    char *fname_in;
    int *ptr_return_value;
        
    //Malloc all the vars!
    ptr_return_value = (int *) calloc(1, sizeof(int));
    
    //Get the file name
    fname_in = mxArrayToString(prhs[0]);
        
    ptr_return_value[0] = biryani_loadFile(fname_in);

    plhs[0] = mxCreateDoubleScalar((double)ptr_return_value[0]);   
}
