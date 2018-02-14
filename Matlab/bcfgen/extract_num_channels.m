function [num_channels_excluding_ref] = extract_num_channels(data)

idx = 1:length(data);
positions = idx(data==4);  %0x04 is end of packet
distances = diff(positions);
num_channels_excluding_ref = (max(distances - 8) / 4);  %Includes reference