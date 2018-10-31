//-------------------------------------------------------------------------------------
// Nathan Paes
// Ryerson Hyperloop
// DAFT Software
// 10/05/2018
//-------------------------------------------------------------------------------------

// Libraries
#include <HX711_ADC.h>

volatile float delta_t;
volatile unsigned long ContactBounceTime = 0; // Timer to avoid contact bounce in isr
double circ = 3.14 / 4;

// Define Sensors
// HX711 constructor (dout pin, sck pin)
HX711_ADC cell_1(2, 3);
HX711_ADC cell_2(4, 5);

// Global Variables
long time_counter;
float load_packet_1;
float load_packet_2;
volatile float speed_packet_1 = 0;
float interval;
int brk_flag = 1;
int digitalPin= 6;

// Setup
void setup() {
  Serial.begin(9600);
  Serial.println(1);
  pinMode(digitalPin, INPUT_PULLUP);
  attachInterrupt(6, speed_sensor, FALLING);
  
  // Variables
  long stabilisingtime = 5000; // tare preciscion can be improved by adding a few seconds of stabilising time
  float sample_rate = 10.000;
  interval = 1 / sample_rate;

  // Start Load Cells
  cell_1.begin();
  cell_2.begin();
  byte loadcell_1_rdy = 0;
  byte loadcell_2_rdy = 0;
  while ((loadcell_1_rdy + loadcell_2_rdy) < 2) { //run startup, stabilization and tare, both modules simultaniously
    if (!loadcell_1_rdy) loadcell_1_rdy = cell_1.startMultiple(stabilisingtime);
    if (!loadcell_2_rdy) loadcell_2_rdy = cell_2.startMultiple(stabilisingtime);
  }
  cell_1.setCalFactor(696.0); // user set calibration factor (float)
  cell_2.setCalFactor(696.0); // user set calibration factor (float)
  time_counter = millis();
}

void loop() {
  if (millis() > (time_counter + interval)) {
    cell_1.update();
    cell_2.update();
    load_packet_1 = cell_1.getData();
    load_packet_2 = cell_2.getData();
    time_counter = millis();
  }

  if (Serial.available() > 0) {
    char inByte = Serial.read();
    switch (inByte) {
      //Tare
      case 't':
        cell_1.tareNoDelay();
        cell_2.tareNoDelay();
        while ((cell_1.getTareStatus() != true) && (cell_2.getTareStatus() != true)) {
          cell_1.update();
          cell_2.update();
          load_packet_1 = cell_1.getData();
          load_packet_2 = cell_2.getData();
        }

        Serial.println(1);
        break;

      //Single Reading
      case 'r':
        Serial.print(load_packet_1);
        Serial.print(";");
        Serial.print(load_packet_2);
        Serial.print(";");
        Serial.print(10);
        Serial.print(";");
        Serial.println(1);
        break;

      // AQUISITION
      case 's':
        Serial.println(1);
        cell_1.tareNoDelay();
        cell_2.tareNoDelay();
        while ((cell_1.getTareStatus() != true) && (cell_2.getTareStatus() != true)) {
          cell_1.update();
          cell_2.update();
          load_packet_1 = cell_1.getData();
          load_packet_2 = cell_2.getData();
        }
        delay(1);
        time_counter = millis();
        while (brk_flag == 1) {
          if (millis() > (time_counter + interval)) {
            cell_1.update();
            cell_2.update();
            load_packet_1 = cell_1.getData();
            load_packet_2 = cell_2.getData();
            time_counter = millis();
            Serial.print(load_packet_1);
            Serial.print(";");
            Serial.print(load_packet_2);
            Serial.print(";");
            Serial.print(speed_packet_1);
            Serial.print(";");
            Serial.println(1);
            //speed_packet_1 = speed_packet_1 + 1;
          }
          // Stop aquisition if end command received
          if (Serial.available() > 0) {
            char inByte = Serial.read();
            if (inByte == 'e') {
              Serial.println(1);
              brk_flag = 0;
            }
          }
        }
        brk_flag = 1;

        break;
      default:
        break;
    }
  }
}

void speed_sensor() {
  if ((millis() - ContactBounceTime) > 100 ) { // debounce the switch contact.
    //Rotations++;
    delta_t = millis() - ContactBounceTime;
    ContactBounceTime = millis();
  }
  speed_packet_1 = circ / delta_t * 1000;
}

