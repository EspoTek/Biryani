#include "o1buffer.h"


//o1buffer is an object that has o(1) access times for its elements.
//At the moment it's basically an array, but I'm keeping it as an object so it can be changed to something more memory efficient later.
//See isobuffer in github.com/espotek/labrador for an example of a much more compact (RAM-wise) buffer.
o1buffer::o1buffer()
{
    buffer = (int *) (malloc(sizeof(int)*NUM_SAMPLES_PER_CHANNEL));
}

o1buffer::~o1buffer(){
    free(buffer);
}


void o1buffer::add(int value, int address){
    //Ensure that the address is not too high.
    if(address >= NUM_SAMPLES_PER_CHANNEL){
        address = address % NUM_SAMPLES_PER_CHANNEL;
    }
    if(address<0){
        fprintf(stderr, "ERROR: o1buffer::add was given a negative address\n");
    }
    //Assign the value
    buffer[address] = value;
    updateMostRecentAddress(address);
}

int o1buffer::get(int address){
    //Ensure that the address is not too high.
    if(address >= NUM_SAMPLES_PER_CHANNEL){
        address = address % NUM_SAMPLES_PER_CHANNEL;
    }
    if(address<0){
        fprintf(stderr, "ERROR: o1buffer::get was given a negative address\n");
    }
    //Return the value
    return buffer[address];
}

void o1buffer::updateMostRecentAddress(int newAddress){
    //qDebug("o1buffer::updateMostRecentAddress(%d)", newAddress);
    if(mostRecentAddress >= (NUM_SAMPLES_PER_CHANNEL-16)){
        mostRecentAddress = newAddress;
    }
    else{
        mostRecentAddress = (newAddress>mostRecentAddress) ? newAddress : mostRecentAddress;
    }
}

//This function places samples in a buffer than can be plotted on the streamingDisplay.
//A small delay, is added in case the packets arrive out of order.
std::vector<double> *o1buffer::getMany_nofilter_double(int numToGet, int interval_samples, int delay_samples){
    //Resize the vector
    convertedStream_double.resize(numToGet);

    //Copy raw samples out.
    int tempAddress;
    for(int i=0;i<numToGet;i++){
        tempAddress = mostRecentAddress - delay_samples - (interval_samples * i);
        if(tempAddress < 0){
            tempAddress += NUM_SAMPLES_PER_CHANNEL;
        }
        double *data = convertedStream_double.data();
        data[i] = buffer[tempAddress];
        //convertedStream_double.replace(i, buffer[tempAddress]);
    }
    return &convertedStream_double;
}

std::vector<double> *o1buffer::getSinceLast(int feasible_window_begin, int feasible_window_end, int interval_samples){
    int feasible_start_point = mostRecentAddress - feasible_window_begin;
    if(feasible_start_point < 0){
        feasible_start_point += NUM_SAMPLES_PER_CHANNEL;
    }

    int actual_start_point;
    printf_debugging("stream_index_at_last_call = %d\nfeasible_start_point=%d\n,mostRecentAddress=%d\n", stream_index_at_last_call, feasible_start_point, mostRecentAddress);
    printf_debugging("distanceFromMostRecentAddress(feasible_start_point) = %d\ndistanceFromMostRecentAddress(stream_index_at_last_call)=%d\n", distanceFromMostRecentAddress(feasible_start_point), distanceFromMostRecentAddress(stream_index_at_last_call));
    if(distanceFromMostRecentAddress(feasible_start_point) > distanceFromMostRecentAddress(stream_index_at_last_call)){
        printf_debugging("Earliest feasible start point is further behind than the stream index at last call\n");
        actual_start_point = stream_index_at_last_call;
    } else {
        printf_debugging("Stream index at last call is further behind than the earliest feasible start point\n");
        actual_start_point = stream_index_at_last_call;
    }
    return NULL;
}

int o1buffer::distanceFromMostRecentAddress(int index){
    //Standard case.  buffer[NUM_SAMPLES_PER_CHANNEL] not crossed between most recent and index's sample writes.
    if(index < mostRecentAddress){
        return mostRecentAddress - index;
    }

    //Corner case.  buffer[NUM_SAMPLES_PER_CHANNEL] boundary has been crossed.
    if(index > mostRecentAddress){
        //Two areas.  0 to mostRecentAddress, and index to the end of the buffer.
        return mostRecentAddress + (NUM_SAMPLES_PER_CHANNEL - index);
    }

    //I guess the other corner case is when the addresses are the same.
    return 0;
}
