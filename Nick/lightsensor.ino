#define digitalPin (5)
//const int analogPin = A0;
volatile int ctr;
unsigned int rpm;
// can use int or float
unsigned long StartTime;
unsigned long EllapsedTime;
unsigned long CurrentTime;
// mills() returns unsigned long
float divisor = 60000;
// has to be float so that decimal point preserved

void setup() 
{
  pinMode(digitalPin, INPUT_PULLUP);
  //pinMode(analogPin, INPUT);
 
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(digitalPin), counter, RISING);
  ctr = 0;
}

void loop() {
  Serial.print("pinValue: ");
  Serial.println(digitalRead(digitalPin));
  Serial.print("Rotations: ");
  Serial.println(ctr);
  if (ctr==5)
  {
    CurrentTime = millis();
    EllapsedTime = CurrentTime - StartTime;
    Serial.print("Ellapsed Time: ");
    Serial.println(EllapsedTime);
    
//    Serial.println(EllapsedTime/divisor);
//    Serial.println(ctr/EllapsedTime/divisor);
//    for debugging^

    rpm = (ctr/(EllapsedTime/divisor));
    Serial.print("RPM: ");
    Serial.println(rpm);
    ctr=0;
  }
  delay(100);
}

void counter() {
  ctr++;
  if (ctr == 1)
  {
  StartTime = millis();
  }
  // Serial.println("Interrupted"); 
  // To see how many times interrupted 
}
