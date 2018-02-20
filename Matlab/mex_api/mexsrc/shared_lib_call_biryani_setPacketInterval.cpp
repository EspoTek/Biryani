//MATLAB INCLUDES
#include <matrix.h>  //Do not include if using GNU Octave to compile mex
#include <mex.h>

//Biryani API Include
#include "biryani.h"
#include <stdio.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //I/O
    double *ptr_numeric_in;
           
    ptr_numeric_in = mxGetPr(prhs[0]); //Mex get pointer to real (double);
    biryani_setPacketInterval((int)(ptr_numeric_in[0]));
}
