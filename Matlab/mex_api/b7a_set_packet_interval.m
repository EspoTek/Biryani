function [] = b7a_set_packet_interval(newPacketInterval)
    if(~isa(newPacketInterval, 'double'))
        error('b7a_set_packet_interval only accepts variables of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end
    shared_lib_call_biryani_getNumChannelsExcludingRef(newPacketInterval);