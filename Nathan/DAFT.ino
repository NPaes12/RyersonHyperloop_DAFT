//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------

// Libraries
#include <HX711_ADC.h>

// Define Sensors
// HX711 constructor (dout pin, sck pin)
// HX711_ADC cell_2(2, 3);
HX711_ADC cell_1(3, 4);

// Global Variables
long t;
float i;
float interval;
int brk_flag = 1;
float spd = 0;

// Setup
void setup() {
  Serial.begin(9600);
  Serial.println(1);
  // Variables
  long stabilisingtime = 5000; // tare preciscion can be improved by adding a few seconds of stabilising time
  float sample_rate = 10.000;
  interval = 1 / sample_rate;
  //Serial.println(interval, DEC);
  // Start Load Cells
  cell_1.begin();
  cell_1.start(stabilisingtime);
  cell_1.setCalFactor(696.0); // user set calibration factor (float)
  // float i;
  t = millis();
}

void loop() {

  if (millis() > (t + interval)) {
    cell_1.update();
    i = cell_1.getData();
    t = millis();
  }

  if (Serial.available() > 0) {
    //float i;
    char inByte = Serial.read();

    switch (inByte) {
      //Tare
      case 't':
        cell_1.tareNoDelay();
        while (cell_1.getTareStatus() != true) {
          cell_1.update();
          i = cell_1.getData();
        }
        Serial.println(1);
        break;

      //Single Reading
      case 'r':
        Serial.print(i);
        Serial.print(";");
        Serial.print(i);
        Serial.print(";");
        Serial.print(10);
        Serial.print(";");
        Serial.println(1);
        break;

      // AQUISITION
      case 's':

        Serial.println(1);
        cell_1.tareNoDelay();
        while (cell_1.getTareStatus() != true) {
          cell_1.update();
          i = cell_1.getData();
        }
        delay(1);
        t = millis();
        while (brk_flag == 1) {

          if (millis() > (t + interval)) {
            cell_1.update();
            i = cell_1.getData();
            t = millis();

            Serial.print(i);
            Serial.print(";");
            Serial.print(i);
            Serial.print(";");
            Serial.print(spd);
            Serial.print(";");
            Serial.println(1);
            spd = spd + 1;
          }
          // Stop aquisition if end command received
          if (Serial.available() > 0) {
            //float i;
            char inByte = Serial.read();
            if (inByte == 'e') {
              Serial.println(1);
              brk_flag = 0;
            }
          }
        }
        brk_flag=1;

        break;
      default:
        // statements
        break;
    }
  }
}

