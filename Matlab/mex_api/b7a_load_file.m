function [retVal] = b7a_load_file(fname)
    if(isa(fname, 'string'))
        fname = char(fname);
    end

    if(~isa(fname, 'char'))
        error('b7a_load_file only accepts variables of types string or char array.\nChar array (single quote) is recommended.\n');
    end
    
    retVal = shared_lib_call_biryani_loadFile(fname);