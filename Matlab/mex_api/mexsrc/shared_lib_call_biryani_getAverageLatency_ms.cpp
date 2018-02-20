//MATLAB INCLUDES
#include <matrix.h>  //Do not include if using GNU Octave to compile mex
#include <mex.h>

//Biryani API Include
#include "biryani.h"
#include <stdio.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //I/O
    double *ptr_return_value;
        
    //Malloc all the vars!
    ptr_return_value = (double *) calloc(1, sizeof(double));
    
    ptr_return_value[0] = biryani_getAverageLatency_ms();

    plhs[0] = mxCreateDoubleScalar(ptr_return_value[0]);   
}
