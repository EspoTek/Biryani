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
b7a_load_file('32ch_20k_noemg.bcf')

%This next step is optional.  It gets a "packet interval" variable than can
%optimise latency.
packet_interval = b7a_get_packet_interval()

%We're going to manually override the packet interval in the API.  This
%will reduce latency but may cause instability.  Be careful and test your
%code!
b7a_set_packet_interval(floor(packet_interval/5))

%Next we initialise the Synamps 2 device.  This will get it out of
%"standby mode" if it's already in there,
if (b7a_initialise_synamps2() == 0) %0 means that it was in standby but now isn't.
    pause(5)  %Give the amplifier time to connect to the USB bus
end

%Finally, we can start the stream.
b7a_start_stream()

%Set up some variables.
downSampling_frequency = 1000 %Hz
filterMode = 1 %Moving Average filter.  We could reduce latency further slightly by setting this to 0
delay = 0.01 %seconds
timeWindow = 0.05 %seconds

%Pause a bit, before we start plotting.
pause((timeWindow + delay) * 1.5);

%Now we plot.
ymin = inf;
ymax = -inf;

xaxis = linspace(-(delay + timeWindow), -delay, downSampling_frequency * timeWindow);
for(i=1:1024)
    tic
    fprintf('\nDisplaying all channels, iteration %d\n', i);
    %This will get 1 second of data from 0.1 seconds in the past,
    %downsampled to 2kHz with no filter.
    cool = b7a_get_data_all_channels_recent(downSampling_frequency, filterMode, delay, timeWindow);
    
    %%Discard the samples from the reference channel
    cool = cool(:, 1:end-1);
      
    %%Show how long we wait between packets
    fprintf('Latency due to internal buffering is %.2fms\n', b7a_get_latency_ms())
    
    %Set the range of the graph.
    if(min(min(cool))<ymin)
        ymin = min(min(cool)) - 1;
    end
    if(max(max(cool)) > ymax)
        ymax = max(max(cool)) + 1;
    end
    
    %Plot it
    plot(xaxis, cool);
    axis([min(xaxis), max(xaxis), ymin, ymax]);
    
    %%Force it to draw now
    drawnow
    toc
end
%Before we quit, we stop the stream.
b7a_stop_stream()