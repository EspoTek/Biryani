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
b7a_load_file('20k32chtest.bcf')

%Next we initialise the Synamps 2 device.  This will get it out of
%"standby mode" if it's already in there,
if (b7a_initialise_synamps2() == 0) %0 means that it was in standby but now isn't.
    pause(5)  %Give the amplifier time to connect to the USB bus
end

%Finally, we can start the stream.
b7a_start_stream()

pause(3)

for i = 1:10 
    pause(i)
    i
    tic
    cool = b7a_get_data_all_channels_sinceLast(1000, 0, 0.01, inf);
    toc
    plot(cool(:,1:end-1));
    drawnow
end