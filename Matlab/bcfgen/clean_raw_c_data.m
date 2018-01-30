function [cleanString] = clean_raw_c_data(rawPacketString)

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