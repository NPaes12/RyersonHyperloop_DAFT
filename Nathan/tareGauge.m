function [ack]=tareGauge(due)
brk=1;
while brk
    fprintf(due,'t');
    msg=fscanf(due);
    if contains(msg,'ack')
        brk=0;
        ack=1;
    end
end
end