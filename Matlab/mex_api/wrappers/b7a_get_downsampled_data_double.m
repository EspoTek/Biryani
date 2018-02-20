function [outputArray] = b7a_get_downsampled_data_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds)
    
    if(~(channel==floor(channel)))
        error('Value of channel must be a whole number');
    end
    
    if(~(filter_mode==floor(filter_mode)))
        error('Value of filter_mode must be a whole number');
    end
    
    if(~isa(sampleRate_hz, 'doublt'))
        error('sampleRate_hz must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end
    
    if(~isa(delay_seconds, 'doublt'))
        error('delay_seconds must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end

    if(~isa(timeWindow_seconds, 'doublt'))
        error('timeWindow_seconds must be of type double.\n(non-programmers, this means a real, numeric value stored in double precision.  Type ''help double'' in the console for more info.)');
    end  
    
    outputArray = shared_lib_call_biryani_getDownSampledChannelData_double(channel, sampleRate_hz, filter_mode, delay_seconds, timeWindow_seconds);