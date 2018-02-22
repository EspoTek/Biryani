//MATLAB INCLUDES
#include <matrix.h>  //Do not include if using GNU Octave to compile mex
#include <mex.h>

//Biryani API Include
#include "biryani.h"
#include <stdio.h>
#include <vector>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    //I/O
    double *ptr_sampleRate_hz_in;
    double *ptr_filter_mode_in;
    double *ptr_delay_seconds_in;
    double *ptr_timeWindow_seconds_in;
    //double *ptr_length_out;
    double *ptr_data_out;
    mwSize dims[2] = {0,0};
    int ndims = 2;
    
    //Internals
    double sampleRate_hz;
    int filter_mode;
    double delay_seconds;
    double timeWindow_seconds;
    int* length_int;
    std::vector<double>** dll_return;
    
    //Fetch inputs.
    ptr_sampleRate_hz_in = mxGetPr(prhs[0]);
    ptr_filter_mode_in = mxGetPr(prhs[1]);
    ptr_delay_seconds_in = mxGetPr(prhs[2]);
    ptr_timeWindow_seconds_in = mxGetPr(prhs[3]);
    //ptr_length_in = mxGetPr(prhs[5]);
    
    //Copy to internals
    sampleRate_hz = (*ptr_sampleRate_hz_in);
    filter_mode = (int)(*ptr_filter_mode_in);
    delay_seconds = (*ptr_delay_seconds_in);
    timeWindow_seconds = (*ptr_timeWindow_seconds_in);
    length_int = (int*)mxCalloc(1, sizeof(int));
    
    //Work out how many channels we're servicing
    int numChannelsIncludingRef = biryani_getNumChannelsExcludingRef() + 1;

    //Actually get the data.
    dll_return = getData_allChannels_sinceLastCall(sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length_int);
   
    //Abort now if there's no data!!!
    if(dll_return == NULL){
        dims[0] = 1;
        dims[1] = 1;
        plhs[0] = mxCreateNumericArray(ndims, dims, mxDOUBLE_CLASS, mxREAL);
        double *ptr_zero = (double *) mxCalloc(1, sizeof(double));
        ptr_zero[0] = 0;
        mxSetData(plhs[0], ptr_zero);
        return;
    }
    
    //Malloc all the outputs!
    //We need to copy from the DLL to MATLAB so that it can work with the data without cracking the sads.
    ptr_data_out = (double*)mxCalloc(length_int[0] * numChannelsIncludingRef, sizeof(double));
    for (int i = 0; i < numChannelsIncludingRef; i++){
        memcpy(&ptr_data_out[i*(length_int[0])], dll_return[i]->data(), length_int[0] * sizeof(double));
    }
    dims[0] = numChannelsIncludingRef;
    dims[1] = length_int[0];
    
    //plhs[0] = mxCreateDoubleScalar((double)ptr_length_out[0]);  
    plhs[0] = mxCreateNumericMatrix(dims[1], dims[0], mxDOUBLE_CLASS, mxREAL);
    mxSetData(plhs[0], ptr_data_out);
}
