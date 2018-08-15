function [logTime,logForce,logSpeed,ack]=runAqDAFT(due)

% File Path
myPrimaryPath='C:\Users\nathan\Desktop\Hyperloop\DAFT';
testName=input('Enter Test Name: ','s');
rootPath = fullfile(myPrimaryPath,testName);
timeStamp=datestr(now,'dd_mm_yy HH_MM');

if ~exist(rootPath)                     % If the directories dont exist create them
    mkdir(rootPath);
end

% Variables
aqTime=300;                             % s
freq=10;                                % Hz
numS=aqTime*freq;                       % Number of samples
interval=1/freq;                        % Time between samples

logTime=zeros(aqTime*freq,1);           % Preallocate to speed up data logging
logSpeed=zeros(aqTime*freq,1);          % Preallocate to speed up data logging
logForce=zeros(aqTime*freq,2);          % Preallocate to speed up data logging

calFact1=500;                           % precalibrated values for gauges 1 and 2
calFact2=500;

% Connect to arduino and start internal program
disp('Connecting to Arduino...')
[due,ack]=openArduinoSerialPort(comPort);

% Tare gauge
disp('Zeroing Gauge... Hold Stll')
[ack]=tareGauge(due);

% Set Cal Factor
disp('Setting Calibration Factor...')
[ack]=setCalFactor(due,1,calFact1);
[ack]=setCalFactor(due,2,calFact2);

% Trigger aquisiton
input('Hit enter to start aquisition\n');
fprintf(due,'runAq');
disp('Begining Aqusition')
ctr=1;

while ctr<numS
    if toc>=interval
        tic
        msg=fscanf(due);
        sep=strfind(msg,'_');
        logTime(ctr,:)=str2double(msg(1:(sep(1)-1)));
        logForce(ctr,1)=str2double(msg((sep(1)+1):(sep(2)-1)));
        logForce(ctr,2)=str2double(msg((sep(2)+1):(sep(3)-1)));
        logSpeed(ctr,:)=str2double(msg((sep(3)+1):end));
        ctr=ctr+1;
    end
end
fprintf(due,'stop');
disp('Aquisition Complete')

% Save data to file
save(fullfile(rootPath,['testName_',timeStamp,'.mat']),'logTime','logForce','logSpeed');
end