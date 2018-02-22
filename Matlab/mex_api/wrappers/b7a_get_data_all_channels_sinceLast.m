function [outputMatrix] = b7a_get_data_all_channels_sinceLast(sampleRate_hz, filter_mode, delay_seconds, timeWindowMax_seconds)
       
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
    
    outputMatrix = shared_lib_call_biryani_getData_allChannels_sinceLastCall(sampleRate_hz, filter_mode, delay_seconds, timeWindowMax_seconds);