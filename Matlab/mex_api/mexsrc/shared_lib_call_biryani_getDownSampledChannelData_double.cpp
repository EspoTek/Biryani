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
    double *ptr_channel_in;
    double *ptr_sampleRate_hz_in;
    double *ptr_filter_mode_in;
    double *ptr_delay_seconds_in;
    double *ptr_timeWindow_seconds_in;
    //double *ptr_length_out;
    double *ptr_data_out;
    mwSize dims[2] = {0,0};
    
    //Internals
    int channel;
    double sampleRate_hz;
    int filter_mode;
    double delay_seconds;
    double timeWindow_seconds;
    int* length_int;
    std::vector<double>* dll_return;
    
    //Fetch inputs.
    ptr_channel_in = mxGetPr(prhs[0]); //Mex get pointer to real (double);
    ptr_sampleRate_hz_in = mxGetPr(prhs[1]);
    ptr_filter_mode_in = mxGetPr(prhs[2]);
    ptr_delay_seconds_in = mxGetPr(prhs[3]);
    ptr_timeWindow_seconds_in = mxGetPr(prhs[4]);
    //ptr_length_in = mxGetPr(prhs[5]);
    
    //Copy to internals
    channel = (int)(*ptr_channel_in);
    sampleRate_hz = (*ptr_sampleRate_hz_in);
    filter_mode = (int)(*ptr_filter_mode_in);
    delay_seconds = (*ptr_delay_seconds_in);
    timeWindow_seconds = (*ptr_timeWindow_seconds_in);
    length_int = (int*)mxCalloc(1, sizeof(int));

    //Actually get the data.
    dll_return = biryani_getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds, length_int);

    //Malloc all the outputs!
    //ptr_length_out = (double*)mxCalloc(1, sizeof(double));
    //ptr_length_out[0] = (double)(length_int[0]);
    ptr_data_out = dll_return->data();
    dims[0] = 1;
    dims[1] = length_int[0];
    
    
    //plhs[0] = mxCreateDoubleScalar((double)ptr_length_out[0]);  
    plhs[0] = mxCreateNumericArray(2, dims, mxUINT8_CLASS, mxREAL);
    mxSetData(plhs[0], ptr_data_out);
}
