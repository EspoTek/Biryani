fprintf("\n\n\nCompiling ignoreme.c...\n\n\n");
mex mexsrc/ignoreme.c %-IC/build_linux/libusb -lusb-1.0 -Lbin\lib\x64
copyfile ignoreme.mexw64 mexbin
delete ignoreme.mexw64
