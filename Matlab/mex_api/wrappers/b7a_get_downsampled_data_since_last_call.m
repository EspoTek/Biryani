function [outputArray] = b7a_get_downsampled_data_since_last_call(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindowMax_seconds)
    
    if(mod(channel, 1)~=0)
        error('Value of channel must be a whole number');
    end
    
    if(mod(filter_mode, 1)~=0)
        error('Value of filter_mode must be a whole number');
    end
    
    if(~isa(sampleRate_hz, 'double'))
        error('sampleRate_hz must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end
    
    if(~isa(delay_seconds, 'double'))
        error('delay_seconds must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end

    if(~isa(timeWindowMax_seconds, 'double'))
        error('timeWindowMax_seconds must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end  
    
    outputArray = shared_lib_call_biryani_getAllDownSampledChannelDataSinceLastCa(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindowMax_seconds);