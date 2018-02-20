function [retVal] = b7a_get_latency_ms()
    retVal = shared_lib_call_biryani_getAverageLatency_ms();