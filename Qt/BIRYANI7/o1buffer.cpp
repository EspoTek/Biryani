#include "o1buffer.h"
#include "userdefinedproperties.h"
#include <QDebug>

//o1buffer is an object that has o(1) access times for its elements.
//At the moment it's basically an array, but I'm keeping it as an object so it can be changed to something more memory efficient later.
//See isobuffer in github.com/espotek/labrador for an example of a much more compact (RAM-wise) buffer.
o1buffer::o1buffer(QObject *parent) : QObject(parent)
{
    buffer = (int *) (malloc(sizeof(int)*NUM_SAMPLES_PER_CHANNEL));
    convertedStream.resize(MAX_CONVERTED_SAMPLES_STORED);
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
    updateMostRecentAddress(address);
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

void o1buffer::updateMostRecentAddress(int newAddress){
    //qDebug("o1buffer::updateMostRecentAddress(%d)", newAddress);
    if(mostRecentAddress = 0x00ffffff){
        mostRecentAddress = newAddress;
    }
    else{
        mostRecentAddress = (newAddress>mostRecentAddress) ? newAddress : mostRecentAddress;
    }
}

//This function places samples in a buffer than can be plotted on the streamingDisplay.
//A small delay, is added in case the packets arrive out of order.
QVector<double> *o1buffer::getMany_nofilter(int numToGet, int interval_samples, int delay_samples){
    int tempAddress;
    for(int i=0;i<numToGet;i++){
        tempAddress = mostRecentAddress - delay_samples - (interval_samples * i);
        if(tempAddress < 0){
            tempAddress += NUM_SAMPLES_PER_CHANNEL;
        }
        convertedStream.replace(i, buffer[tempAddress]);
    }
    return &convertedStream;
}
