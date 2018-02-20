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

% % % % % BIRYANI_API std::vector<double>* biryani_getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);










































% //Non-API
% % % % % BIRYANI_API double fnbiryani(double number_in);
% % % % % BIRYANI_API int biryani_enable_debugging_console();
% % % % % BIRYANI_API int biryani_init();
% //Generic Getters
% % % % % BIRYANI_API int biryani_getNumChannelsExcludingRef();
% % % % % BIRYANI_API int biryani_getPacketInterval();
% % % % % BIRYANI_API double biryani_getAverageLatency_ms();
% //Generic Setters
% % % % % BIRYANI_API void biryani_setDebugLevel(int new_debug_level_in);
% % % % % BIRYANI_API void biryani_setPacketInterval(int new_packet_interval);
% //Synamps Specific Functions
% % % % % BIRYANI_API int biryani_loadFile(char *fname);
% % % % % BIRYANI_API int biryani_initialiseSynamps2Device();
% % % % % BIRYANI_API int biryani_stopStream();
% % % % % BIRYANI_API int biryani_startStream();
% //Other Functions
% % % % % BIRYANI_API std::vector<double>* biryani_getDownSampledChannelData_double(int channel, double sampleRate_hz, int filter_mode, double delay_seconds, double timeWindow_seconds, int* length);
