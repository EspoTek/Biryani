//MATLAB INCLUDES
#include <matrix.h>  //Do not include if using GNU Octave to compile mex
#include <mex.h>

//Biryani API Include
#include "biryani.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //I/O
    double *ptr_number_in;
    double *ptr_number_out;
    
    //Malloc all the vars!
    ptr_number_out = (double *) calloc(1, sizeof(double));
    
    ptr_number_in = mxGetPr(prhs[0]); //Mex get pointer to real (double);
    ptr_number_out[0] = fnbiryani(ptr_number_in[0]);

    plhs[0] = mxCreateDoubleScalar((double) ptr_number_out[0]);   
}
