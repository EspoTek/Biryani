function [outputString] = format_cleaned_data(cleanString)

%Variables for phase tracking
phase2str = {'PHASE_1_CONFIG', 'PHASE_2_DATA', 'PHASE_3_CLEANUP'};
currentPhase = 1;
dataPhaseCharacterised = 0;

%Get the indices for the relevant "special" characters.
num_packets = length(strfind(cleanString, 'pkt'));
openBrackets = strfind(cleanString, '[') + 1;
closeBrackets = strfind(cleanString, ']') - 1;
openCurlys = strfind(cleanString, '{') + 1;
closeCurlys = strfind(cleanString, '}') - 1;

%Find the length of each packet
fprintf('\nChecking length of Phase 2 (Data) packets.\n');
packet_lengths = [];
for i = 1:num_packets
    %Extract the packet length field
    len_string = cleanString(openBrackets(i) : closeBrackets(i));
    len_numeric = str2double(len_string);
    packet_lengths = [packet_lengths len_numeric];
end

%Find the length of the phase 2 (data) packets only
data_packet_length = 0;
for test_index = 128:length(packet_lengths) %begin the search at packet 128, to ensure we skip phase 1
    if(max(packet_lengths(test_index-10:test_index)) == min(packet_lengths(test_index-10:test_index))) %ie, they're all the same
        data_packet_length = packet_lengths(test_index);
    end
end
if(data_packet_length == 0)
    error('Could not find the phase 2 packet length.  Aborting...');
else
    fprintf('Phase 2 packets have a length of %d\n', data_packet_length);
end

%Initialise output strings
headerString = sprintf('Biryani 7 Configuration File Version 0.9\n');
phase1String = char([]);
phase2String = [phase2str{2} ' ' num2str(data_packet_length) sprintf('\n')];
phase3String = char([]);

num_lines = 1; %1 line for the phase 2 information.  Other lines added in the loop.

fprintf('Formatting data to be compatible with Biryani 7...');
%Format data and write to output string
for i = 1:num_packets
    len_numeric = packet_lengths(i);
    
    %Extract the packet data
    packetDataString_raw = cleanString(openCurlys(i):closeCurlys(i));
    packetDataString_cleaned = erase(packetDataString_raw, '0x');  
    
    format_string = char([]);
    for temp = 1:(len_numeric-1)
        format_string = [format_string '%x, '];
        %Speedup.  Only the USBPcap header is needed for phase 2 transfers.
        if((currentPhase == 2) && (temp>27) && (len_numeric == data_packet_length))
            break;
        end
    end
    format_string = [format_string '%x'];
    
    packetData = sscanf(packetDataString_cleaned, format_string)';
    
    %Check for phase change
    %134 dec = 86 hex.  An IN transfer from EP6 of length data_packet_length signifies beginning of phase 2.
    if((currentPhase == 1) && (packetData(22) == 134) && (len_numeric == data_packet_length))
        currentPhase = 2;
    end
    %Anything that _isn't_ an IN transfer from EP6 of length data_packet_length signifies beginning of phase 3.
    if((currentPhase == 2) && ((packetData(22) ~= 134) || (len_numeric ~= data_packet_length)))
        currentPhase = 3;
    end

    
    %Write to the appropriate output substring.
    switch(currentPhase)
        case 1
            packetData_output = char([]);
            for current_byte = 1:len_numeric
                packetData_output = [packetData_output dec2hex(packetData(current_byte)) ' '];
            end
            phase1String = [phase1String phase2str{1} ' ' num2str(len_numeric) ' ' packetData_output sprintf('\n')];
            num_lines = num_lines + 1;
        case 2
            %nothing!
        case 3
            packetData_output = char([]);
            for current_byte = 1:len_numeric
                packetData_output = [packetData_output dec2hex(packetData(current_byte)) ' '];
            end
            phase3String = [phase3String phase2str{3} ' ' num2str(len_numeric) ' ' packetData_output sprintf('\n')];
            num_lines = num_lines + 1;
        otherwise 
            error('Cannot write to output string.  Phase invalid!')
    end
end

numLinesString = sprintf('NUM_LINES %d\n', num_lines);

fprintf('   Complete!');
outputString = [headerString numLinesString phase1String phase2String phase3String];