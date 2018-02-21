function [retVal] = b7a_get_sample_rate_hz()
    retVal = shared_lib_call_biryani_measureSampleRate();