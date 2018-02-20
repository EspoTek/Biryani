function [] = recompile_cpp(fname_no_dot_cpp)

fprintf("\n\n\nCompiling %s.cpp...\n\n\n", fname_no_dot_cpp);

mex('-I../../cpp-generic/src/', '-I../../DLL/biryani/biryani', '-lbiryani', '-L../../DLL/biryani/x64/Release', ['mexsrc/' fname_no_dot_cpp '.cpp'])
copyfile([fname_no_dot_cpp '.mexw64'], 'mexbin')
delete([fname_no_dot_cpp '.mexw64'])

return;
% 
% fprintf("\n\n\nCompiling shared_lib_call_biryani_init.cpp...\n\n\n");
% mex mexsrc/shared_lib_call_biryani_init.cpp -I../../cpp-generic/src/ -I../../DLL/biryani/biryani -lbiryani -L../../DLL/biryani/x64/Release
% copyfile shared_lib_call_biryani_init.mexw64 mexbin
% delete shared_lib_call_biryani_init.mexw64
