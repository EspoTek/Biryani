#include "o1buffer.h"

//o1buffer is an object that has o(1) access times for its elements.
//At the moment it's basically an array, but I'm keeping it as an object so it can be changed to something more memory efficient later.
//See isobuffer in github.com/espotek/labrador for an example of a much more compact (RAM-wise) buffer.
o1buffer::o1buffer(QObject *parent) : QObject(parent)
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
        qFatal("o1buffer::add was given a negative address");
    }
    //Assign the value
    buffer[address] = value;
}

int o1buffer::get(int address){
    //Ensure that the address is not too high.
    if(address >= NUM_SAMPLES_PER_CHANNEL){
        address = address % NUM_SAMPLES_PER_CHANNEL;
    }
    if(address<0){
        qFatal("o1buffer::get was given a negative address");
    }
    //Return the value
    return buffer[address];
}
