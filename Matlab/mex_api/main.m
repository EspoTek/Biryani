clear all
clc

addpaths
recompile

copyfile ../../DLL/biryani/x64/Release/biryani.dll

return;

pause(5);

biryani_7_api_open_debug_console()
biryani_7_api_ignoreme(420);
biryani_7_api_ignoreme_too(420);
biryani_7_api_ignoreme_three();