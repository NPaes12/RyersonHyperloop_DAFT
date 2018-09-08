function [ack]=tareGauge(due)
fprintf(due,'t');
pause(5)
ack=fscanf(due,'%d');

if isempty(ack)
    ack=0;
end

end