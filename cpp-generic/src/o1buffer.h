#ifndef O1BUFFER_H
#define O1BUFFER_H

#include "api_defines.h"

class o1buffer
{
public:
    explicit o1buffer();
    ~o1buffer();
    void add(int value, int address);
    int get(int address);
    int mostRecentAddress = 0;
    std::vector<double> *getMany_nofilter_double(int numToGet, int interval_samples, int delay_sample);
private:
    int *buffer;
    std::vector<double> convertedStream_double;
    void updateMostRecentAddress(int newAddress);
};

#endif // O1BUFFER_H
