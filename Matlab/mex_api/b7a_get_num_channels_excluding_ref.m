function [retVal] = b7a_get_num_channels_excluding_ref()
    retVal = shared_lib_call_biryani_getNumChannelsExcludingRef();