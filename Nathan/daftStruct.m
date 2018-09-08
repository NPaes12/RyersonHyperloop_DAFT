function [obj]=daftStruct()

% Cell Definition
obj.Cell_1.CalibrationValue=1000;
obj.Cell_1.Force='Lift';
obj.Cell_1.Unit='N';
obj.Cell_1.Log=zeros(200,1);

% Cell Definition
obj.Cell_2.CalibrationValue=1000;
obj.Cell_2.Force='Drag';
obj.Cell_2.Unit='N';
obj.Cell_2.Log=zeros(200,1);

% Sensor Definition
obj.SpeedSensor.CalibrationValue=1000;
obj.SpeedSensor.Unit='rot/s';
obj.SpeedSensor.Log=zeros(200,1);




end