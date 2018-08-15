//-------------------------------------------------------------------------------------
//
//
//
//
//
//-------------------------------------------------------------------------------------
//
//

#include <HX711_ADC.h>

//HX711 constructor (dout pin, sck pin)
HX711_ADC LoadCell_1(A1, A0); //HX711 1
//HX711_ADC LoadCell_2(4, 5); //HX711 2

long t;
//*************************************************************************************
//*************************************************************************************
void setup() {
  Serial.begin(9600);
  Serial.println("Waiting for host...");

  // Connect to matlab
  int brk = 1;
  while (brk == 1) {

    if (Serial.available() > 0)
    {
      //read in char
      char inByte = Serial.read();

      if (inByte == 'a')
      {
        Serial.println("ack");
        brk = 0;
      }
      else
      {
        delay(100);
      }
    }
  }

  // Tare Gauges
  int brk = 1;
  while (brk == 1) {

    if (Serial.available() > 0)
    {
      //read in char
      char inByte = Serial.read();

      if (inByte == 't')
      {
        LoadCell_1.tareNoDelay();
        //LoadCell_2.tareNoDelay();
        LoadCell_1.begin();
        //LoadCell_2.begin();
        Serial.println("ack");
        brk = 0;
      }
      else
      {
        delay(100);
      }
    }
  }
}

long stabilisingtime = 5000; // tare preciscion can be improved by adding a few seconds of stabilising time
//byte loadcell_1_rdy = 0;
//byte loadcell_2_rdy = 0;
//while ((loadcell_1_rdy+ loadcell_2_rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously //
//if (!loadcell_1_rdy) loadcell_1_rdy = LoadCell_1.startMultiple(stabilisingtime);
//if (!loadcell_2_rdy) loadcell_2_rdy = LoadCell_2.startMultiple(stabilisingtime);
//}
LoadCell_1.start(stabilisingtime);//
LoadCell_1.setCalFactor(696.0); // user set calibration factor (float)
//LoadCell_2.setCalFactor(733.0); // user set calibration factor (float)
Serial.println("Startup + tare is complete");
}

//*************************************************************************************
//*************************************************************************************
void loop() {
  int endProgram = 1;
  while endProgram == 1
{
  // Wait for signal to start
  int brk = 1;
  while (brk == 1) {

      if (Serial.available() > 0)
      {
        //read in char
        char inByte = Serial.read();

        if (inByte == 's')
        {
          Serial.println("ack");
          brk = 0;
        }
        else
        {
          delay(100);
        }
      }
    }

    //update() should be called at least as often as HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS
    //longer delay in scetch will reduce effective sample rate (be carefull with delay() in loop)
    LoadCell_1.update();
    //LoadCell_2.update();

    //get smoothed value from data set + current calibration factor
    if (millis() > t + 100) {
      float a = LoadCell_1.getData();
      //float b = LoadCell_2.getData();
      //Serial.print("Load_cell 1 output val: ");
      Serial.print(a);
      Serial.print(":");
      Serial.print(millis());
      //Serial.print("    Load_cell 2 output val: ");
      //Serial.println(b);
      t = millis();
    }

    //receive from serial terminal
    if (Serial.available() > 0) {
      float i;
      char inByte = Serial.read();
      if (inByte == 't') {
        LoadCell_1.tareNoDelay();
        //LoadCell_2.tareNoDelay();
      }
    }

    //check if last tare operation is complete
    if (LoadCell_1.getTareStatus() == true) {
      Serial.println("Tare load cell 1 complete");
    }
    //if (LoadCell_2.getTareStatus() == true) {
    // Serial.println("Tare load cell 2 complete");
    //}
    
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'e') {
        endProgram = 0;
      }
    }
  }



}

}
