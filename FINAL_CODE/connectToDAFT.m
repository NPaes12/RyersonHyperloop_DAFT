function [due,ack]=connectToDAFT(comPort)

if nargin<1
    comPort='com9';
end

try
    due = serial(comPort,'BaudRate', 9600);
    fopen(due);
    pause(3)
    %ack=1;
    ack=fscanf(due,'%d');
catch
    ack=0;
    
end

end