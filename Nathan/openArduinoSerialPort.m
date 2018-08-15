function [due,ack]=openArduinoSerialPort(comPort)

if nargin<1
    comPort='com9';
end
due = serial(comPort,'BaudRate', 9600);
fopen(due);
pause(3)

brk=1;
while brk
    fprintf(due,'a');
    msg=fscanf(due);
    if contains(msg,'ack')
        brk=0;
        ack=1;
    end   
end
end