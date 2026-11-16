/*
  Simple character read.  This is looking for an "H" or and "L"
  and will set the LED tied to the given pin on or off.

  "SoftwareSerial" is a library that does a bit-bang USART interface on 
  any two digital GPIOs on an arduino.  This allows you to leave the 
  primary USART for debugging.

  This routine setups a USART on pins 2 and 4 named "bluetooth".  It then
  setups up this uart at 9600 baud.  In the loop, it checks to see if a
  character has arrived, and then reads it.

  The LED13 (built in LED) is blinked to show the program is running.  This
  blink can be moved inside the bluetooth if statement for debugging, or
  the normal serial pipe can be used.
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <ZumoShield.h>


// these might need to be tuned for different motor types
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     200
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON); // pushbutton on pin 12

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

//receiver
int led = 13; 
unsigned char incomingByte;     

// HC-05
SoftwareSerial bluetooth(2,4);  // RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("Serial setup complete.");
  pinMode(led, OUTPUT);
  // Setup the HC-05 bluetooth device
  // (HC)=(Uno).  VCC=5V, GND=GND, TX=RX(pin2),
  // RX=DNC! (possible damage if 5V Uno TX drives to HC-05)
  bluetooth.begin(9600);
}

void loop() {
  digitalWrite(led,HIGH);
  delay(1);
  digitalWrite(led,LOW);
  delay(1);
  if (bluetooth.available()) {  
    incomingByte = (unsigned char) bluetooth.read();
    Serial.write(incomingByte);


    //movement polling
    if(incomingByte=='U'){
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    }
    else if(incomingByte=='D'){
      motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    }
    else if(incomingByte=='L'){
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    }
    else if(incomingByte=='R'){
      motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    }
    else if(incomingByte=='0'){
      motors.setSpeeds(0, 0);
    }
    else{
      motors.setSpeeds(0, 0);
    }

  }
  
}
