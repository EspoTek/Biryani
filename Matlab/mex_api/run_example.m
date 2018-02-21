addpaths

%First we need to call b7a_init() to create an API object.
%You cannot call anything else until this is done.
if(b7a_init() == -1)
    %-1 means the api already exists, so we'll reset it.
    %This is not strictly necessary but I just want to show off what the
    %API can do.
    b7a_reset()
end

%Next, we load our BCF file.
b7a_load_file('8chtest.bcf')

%This next step is optional.  It gets a "packet interval" variable than can
%optimise latency.
b7a_get_packet_interval()

%Next we initialise the Synamps 2 device.  This will get it out of
%"standby mode" if it's already in there,
if (b7a_initialise_synamps2() == 0) %0 means that it was in standby but now isn't.
    pause(5)  %Give the amplifier time to connect to the USB bus
end

%Finally, we can start the stream.
b7a_start_stream()
b7a_get_sample_rate_hz()
%Pause a bit, before we start plotting.
%Needs to be at least 1.1 seconds since that's the plot length.
pause(3);

%Now we plot.
ymin = inf;
ymax = -inf;
%xaxis = linspace(-1.1, 0.1, 2000);
for(i=1:10)
    tic
    fprintf('Displaying channel 0, iteration %d\n', i);
    ymin
    ymax
    %This will get 1 second of data from 0.1 seconds in the past,
    %downsampled to 2kHz with no filter.
    cool = b7a_get_downsampled_data_since_last_call(0, 1000, 0, 8, 0.01);
    
    b7a_get_latency_ms()
    if(min(cool)<ymin)
        ymin = min(cool) - 1;
    end
    
    if(max(cool) > ymax)
        ymax = max(cool) + 1;
    end
    
    %plot(xaxis, cool);
    plot(cool);
    %axis([-1.1, 0.1, ymin, ymax]);
    axis([0, length(cool), ymin, ymax]);
    
    pause(i);

    drawnow
    toc
end

b7a_get_sample_rate_hz()
b7a_get_sample_rate_hz()

%Before we quit, we stop the stream.
b7a_stop_stream()