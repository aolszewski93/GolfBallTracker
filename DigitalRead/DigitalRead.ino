/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pin 0 has a pushbutton attached to it. Give it a name:
int IRSensor1 = 5;
int IRSensor2 = 4;
int IRSensor3 = 14;
int IRState1 = 1; //sets pin state to off
int IRState2 = 1; //sets pin state to off
int IRState3 = 1;

void balllife(int IR, int ballnumber) {
  // conditional to tell whether IR is on or off
  if (IR == 0){
    Serial.println(String(ballnumber) +"Ball is Life"); // print out
  }
  else{
    Serial.println(String(ballnumber) +"Ball is Lost"); // inform ball is lost
  }
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(IRSensor1, INPUT_PULLUP);
  pinMode(IRSensor2, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // conditional that sends a message to serial whenver the sensor changes state
  //if (IRState != digitalRead(IRSensor)) {
    
    IRState1 = digitalRead(IRSensor1); //update the pin state
    IRState2 = digitalRead(IRSensor2); //update the pin state
    IRState3 = digitalRead(IRSensor3);
    balllife(IRState1, IRSensor1);
    balllife(IRState2, IRSensor2);
    balllife(IRState3, IRSensor3);
    
  //}
  delay(1000);        // delay in between reads for stability
}
