fprintf("\n\n\nCompiling ignoreme.c...\n\n\n");
mex mexsrc/ignoreme.c -I../../cpp-generic/src/% -lusb-1.0 -Lbin\lib\x64
copyfile ignoreme.mexw64 mexbin
delete ignoreme.mexw64

fprintf("\n\n\nCompiling ignoremetoo.cpp...\n\n\n");
mex mexsrc/ignoremetoo.cpp -I../../cpp-generic/src/ -I../../DLL/biryani/biryani -lbiryani -L../../DLL/biryani/x64/Release
copyfile ignoremetoo.mexw64 mexbin
delete ignoremetoo.mexw64

fprintf("\n\n\nCompiling ignoremethree.cpp...\n\n\n");
mex mexsrc/ignoremethree.cpp -I../../cpp-generic/src/ -I../../DLL/biryani/biryani -lbiryani -L../../DLL/biryani/x64/Release
copyfile ignoremethree.mexw64 mexbin
delete ignoremethree.mexw64
