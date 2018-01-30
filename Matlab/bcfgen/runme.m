%Warning before cleanup
%warningString = sprintf('\n\nThe MATLAB Workspace and Command Window will be cleared if you continue.\n\nPress Ctrl+C to cancel or any key to continue!');
%warning(warningString);
%pause()

%Cleanup
clear all
clc

%Get the file names
fprintf('Loading directory _c_files\n');
fileNameArray_struct = dir('_c_files');  %Read contents of the directory containing the c files
fileNameArray_struct = fileNameArray_struct(3:end);  %Discard the first two, "." and ".." (cdUp).
fileNameArray = cell(1,length(fileNameArray_struct));  %Discard all information apart from the raw file name
for i = 1:length(fileNameArray)
    fileNameArray{i} = fileNameArray_struct(i).name;
end
clear fileNameArray_struct;

if(isempty(fileNameArray))
    error('Error: no files could be found in directory _c_files');
else
    fprintf('%d file(s) found:\n', length(fileNameArray))
    for i = 1:length(fileNameArray)
        fprintf('%s\n', fileNameArray{i});
    end
end

for i = 1:length(fileNameArray)
    fprintf('\nConverting file: %s', fileNameArray{i});
    
    %Load the File
    fprintf('\nLoading file to memory...');
    fid = fopen(['_c_files/' fileNameArray{i}], 'r');
    rawPacketString = fread(fid, '*char')';
    fclose(fid);
    fprintf('   Complete!');
    
    %Clean the data
    fprintf('\nCleaning data...');
    cleaned_string = clean_raw_c_data(rawPacketString);
    fprintf('   Complete!');
    clear rawPacketString
    
    %Format the data
    formatted_string = format_cleaned_data(cleaned_string);
    clear cleaned_string

    %Prepare the output file
    fprintf('\nPreparing .bcf file...');
    outputFileName = fileNameArray{i};
    dotPosition = strfind(outputFileName, '.');
    if(~isempty(dotPosition))     %Remove everything after the dot
        outputFileName = outputFileName(1:dotPosition(1)-1);
    end
    
    %'_c_files/'
    outputFileName = [outputFileName '.bcf'];
    fid = fopen(outputFileName, 'w');
    fprintf('   Complete!');
    
    fprintf('\nSaving the data to file...');
        fwrite(fid, formatted_string);
        fclose(fid);
    fprintf('   Complete!\nData saved to %s', outputFileName);
    
    fprintf('\nConversion Complete!\n');
    
end


%parse_wireshark_dissect('packet_dissect.c', 'synamps_config.h', 'stage2')