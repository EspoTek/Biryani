clear all
clc

addpaths
copyfile ../../DLL/biryani/x64/Release/biryani.dll
copyfile ../../Qt/BIRYANI7/lib/win64/libusb/dll/libusb-1.0.dll

recompile_cpp('shared_lib_call_biryani_init');
recompile_cpp('shared_lib_call_biryani_initialiseSynamps2Device');
recompile_cpp('shared_lib_call_biryani_loadFile');