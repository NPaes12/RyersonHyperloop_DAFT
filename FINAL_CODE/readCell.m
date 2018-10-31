function [cell_1,cell_2,speed,ack]=readCell(due)

fprintf(due,'r');
pause(0.5)
msg=fscanf(due,'%s');
cell_raw=str2double(split(msg,';'));
if size(cell_raw,1)<3
    ack=0;
else
    cell_1=cell_raw(1);
    cell_2=cell_raw(2);
    speed=cell_raw(3);
    ack=cell_raw(4);
end

end