function [ack]=startDAFT(due)
fprintf(due,'s');
pause(0.5)
ack=fscanf(due,'%d');
 
if isempty(ack) || ack==0
    ack=0;
    warning('Failed to Start DAFT Aquisition')
else
    disp('Aquisition Start Success') 
end

end