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
    int stream_index_at_last_call = SUBPACKET_STREAM_START_INDEX;
    int distanceFromMostRecentAddress(int index);
    std::vector<double> *getMany_nofilter_double(int numToGet, int interval_samples, int delay_sample);
    std::vector<double> *getSinceLast(int feasible_window_begin, int feasible_window_end, int interval_samples);
private:
    int *buffer;
    std::vector<double> convertedStream_double;
    void updateMostRecentAddress(int newAddress);
};

#endif // O1BUFFER_H
