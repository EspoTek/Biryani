%First we need to call b7a_init() to create an API object.
%You cannot call anything else until this is done.
b7a_init()

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

%Pause a bit, before we start plotting.
pause(1);

%Now we plot.
ymin = inf;
ymax = -inf;
xaxis = linspace(-1.1, 0.1, 2000);
for(i=1:100)
    tic
    fprintf('Displaying channel 0, iteration %d\n', i);
    %This will get 1 second of data from 0.1 seconds in the past,
    %downsampled to 2kHz with no filter.
    cool = b7a_get_downsampled_data_double(0, 2000, 0, 0.1, 1);
    b7a_get_latency_ms()
    if(min(cool)<ymin)
        ymin = min(cool);
    end
    
    if max(cool > ymax)
        ymax = max(cool);
    end
    
    plot(xaxis, cool);
    axis([-1.1, 0.1, ymin, ymax]);
    
    drawnow
    toc
end

%Before we quit, we stop the stream.
b7a_stop_stream()