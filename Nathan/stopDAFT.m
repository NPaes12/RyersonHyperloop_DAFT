function []=stopDAFT(due)
fprintf(due,'e');
%pause(0.5)
%ack=fscanf(due,'%d');
 
%{
if isempty(ack) || ack==0
    ack=0;
    warning('Failed to Stop DAFT Aquisition')
else
    disp('Aquisition Stop Success')
end
%}
end