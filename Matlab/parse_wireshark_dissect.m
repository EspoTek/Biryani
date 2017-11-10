function [] = parse_wireshark_dissect(fname, outputName_h, identifier)
clear all
clc

%identifier = 'stage2'
%fname = 'emg.contents'
%outputDir = 'C:\Users\cesposito\Documents\Qt\BIRYANI7'
%outputName_h = [outputDir '\synamps_config.h'];

% identifier = 'stage1'
% fname = 'preinit.contents'
% outputDir = 'C:\Users\cesposito\Documents\Qt\BIRYANI7'
% outputName_h = [outputDir '\synamps_preinit.h'];

%Load the file
fid = fopen(fname, 'r');
rawPacketString = fread(fid, '*char')';
fclose(fid);

%Remove unwanted text in comment tags
beginGarbage = strfind(rawPacketString, '/* ');
endGarbage = strfind(rawPacketString, ' */');

cleanString = char(32*ones(1,length(rawPacketString)));

%Copy text, ignoring everything commented out
cleanStringIdx = 1;
for garbageIterator = 1:(length(endGarbage)-1)
    ptr_low = endGarbage(garbageIterator)+3;
    ptr_high = beginGarbage(garbageIterator+1)-1;
    diff = ptr_high - ptr_low;
    cleanString(cleanStringIdx:(cleanStringIdx+diff)) = rawPacketString(ptr_low:ptr_high);
    cleanStringIdx = cleanStringIdx+diff;
end
%Make sure you get everything after the last end comment too.
ptr_low = endGarbage(end)+3;
ptr_high = length(rawPacketString);
diff = ptr_high - ptr_low;
cleanString(cleanStringIdx:(cleanStringIdx+diff)) = rawPacketString(ptr_low:ptr_high);

%Remove whitespace 
eraseIdx = length(cleanString);
while(cleanString(eraseIdx)==char(32))
    eraseIdx = eraseIdx - 1;
end
cleanString = cleanString(1:eraseIdx);

%Get the indices for the relevant "special" characters.
pkts = strfind(cleanString, 'pkt');
openBrackets = strfind(cleanString, '[');
closeBrackets = strfind(cleanString, ']');

%%Create the output string
outputString = char(32*ones(1,length(rawPacketString)));

%Copy text, adding brackets around the packet names
offset = 0;
outputIdx = 1;
for pktIterator = 1:length(pkts)
   ptr_pkt = pkts(pktIterator);
   ptr_openBracket = openBrackets(pktIterator);
   diff = ptr_openBracket - ptr_pkt - 3;
   
   outputString((outputIdx+offset):(ptr_pkt+offset+2)) = cleanString(outputIdx:ptr_pkt+2);
   %outputString(ptr_pkt+offset+3) = '[';
   outputString((ptr_pkt+offset+3):(ptr_pkt+offset+3+diff)) = cleanString(ptr_pkt+3 : ptr_pkt+diff+3);
   outputString(ptr_pkt+offset+3+diff:ptr_pkt+offset+3+diff+length(identifier)) = ['_' identifier];
   outputIdx = ptr_openBracket;
   offset = offset + length(identifier) + 1;
end

%Make sure to get contents of last packet!
outputString(ptr_openBracket+offset:length(cleanString)+offset) = cleanString(ptr_openBracket:end);

%Remove whitespace at end of file
eraseIdx = length(outputString);
while(outputString(eraseIdx)==char(32))
    eraseIdx = eraseIdx - 1;
end
outputString = outputString(1:eraseIdx);

%Find the number of Packets
numPackets = str2num(cleanString(pkts(end)+3:openBrackets(end)-1));

%Append the packet number to the string
outputString = [outputString '#define NUM_WIRESHARK_PACKETS_' identifier ' ' num2str(numPackets) sprintf('\n')];

%Find the lengths of each packet
packetLengthsString = sprintf('static const unsigned int wireshark_packet_lengths_%s[%d]', identifier, numPackets);
packetLengthsString = [packetLengthsString  ' = {'];
for i = 1:numPackets
    tempText = cleanString(openBrackets(i)+1:closeBrackets(i)-1);
    packetLengthsString = [packetLengthsString, tempText];
    if ~(i == numPackets)
        packetLengthsString = [packetLengthsString, ', '];
    end
end

packetLengthsString = [packetLengthsString '};'];
%Append the packets length array to the string
outputString = [outputString packetLengthsString];

packetsPointerText = sprintf('static const unsigned char *wireshark_packets_%s[%d]', identifier, numPackets);
%Create the packets pointer array
packetsPointerText = [packetsPointerText ' = {'];

for i = 1:numPackets
    tempText = sprintf('pkt%d_%s', i, identifier);
    packetsPointerText = [packetsPointerText, tempText];
    if ~(i == numPackets)
        packetsPointerText = [packetsPointerText, ', '];
    end
end
packetsPointerText = [packetsPointerText '};'];

%Append the packets pointer array to the string
outputString = [outputString sprintf('\n') packetsPointerText];

%Write string to file
fid_header = fopen(outputName_h, 'w');
fwrite(fid_header, outputString);
fclose(fid);

