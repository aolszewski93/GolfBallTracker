/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pin 0 has a pushbutton attached to it. Give it a name:
int IRSensor = 5;
int IRState = 1; //sets pin state to off

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(IRSensor, INPUT_PULLUP);
}

// the loop routine runs over and over again forever:
void loop() {
  
  // conditional that sends a message to serial whenver the sensor changes state
  //if (IRState != digitalRead(IRSensor)) {
    
    IRState = digitalRead(IRSensor); //update the pin state
    
    // conditional to tell whether IR is on or off
    if (IRState == 0){
      Serial.println("Ball is Life"); // print out
    }
    else{
      Serial.println("Ball is Lost"); // inform ball is lost
    }
    
  //}
  delay(100);        // delay in between reads for stability
}
