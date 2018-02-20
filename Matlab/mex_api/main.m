clear all
clc

addpaths
copyfile ../../DLL/biryani/x64/Release/biryani.dll
copyfile ../../Qt/BIRYANI7/lib/win64/libusb/dll/libusb-1.0.dll

recompile_cpp('shared_lib_call_biryani_init');
recompile_cpp('shared_lib_call_biryani_initialiseSynamps2Device');
recompile_cpp('shared_lib_call_biryani_loadFile');

recompile_cpp('shared_lib_call_biryani_getAverageLatency_ms');
recompile_cpp('shared_lib_call_biryani_getDownSampledChannelData_double');
recompile_cpp('shared_lib_call_biryani_getNumChannelsExcludingRef');
recompile_cpp('shared_lib_call_biryani_getPacketInterval');
recompile_cpp('shared_lib_call_biryani_setDebugLevel');
recompile_cpp('shared_lib_call_biryani_setPacketInterval');
recompile_cpp('shared_lib_call_biryani_stopStream');
recompile_cpp('shared_lib_call_biryani_startStream');