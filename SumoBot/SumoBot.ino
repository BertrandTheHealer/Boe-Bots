#include <LiquidCrystal.h>
#include <stdio.h>

/*
 * sudo chmod 666 /dev/ttyACM0
 * 
 * Sensors:
 * Arduino          Sensor
 * D7               QTI4 - Far left
 * D6               QTI3 - Mid left
 * D5               QTI2 - Mid right
 * D4               QTI1 - Far right
 * 
 * Servos:
 * Arduino          Servo
 * D13              Left servo
 * D12              Right servo
 */
#include <Servo.h>                           // Use the Servo library (included with Arduino IDE)  

Servo servoL;                                // Define the left and right servos
Servo servoR;
int linePins[] = {2,3};
int linePinStates[] = {0,0};

// Perform these steps with the Arduino is first powered on
void setup()
{
  Serial.begin(9600);                        // Set up Arduino Serial Monitor at 9600 baud
  servoL.attach(13);                         // Attach (programmatically connect) servos to pins on Arduino
  servoR.attach(12);
  servoL.writeMicroseconds(1500);            //stop the motors
  servoR.writeMicroseconds(1500);            //stop the motors
}

// This code repeats indefinitely
void loop()
{
  for (int pin = 0; pin < 2; pin++) {
    pinMode(linePins[pin], OUTPUT);
    digitalWrite(linePins[pin], HIGH);
  }
  //DDRD |= B11110000;                         // Set direction of Arduino pins D4-D7 as OUTPUT
  //PORTD |= B11110000;                        // Set level of Arduino pins D4-D7 to HIGH
  delayMicroseconds(230);                    // Short delay to allow capacitor charge in QTI module
  for (int pin = 0; pin < 2; pin++) {
    // turn the pin on:
    pinMode(linePins[pin], INPUT);
    digitalWrite(linePins[pin], LOW);
  }
  //DDRD &= B00001111;                         // Set direction of pins D4-D7 as INPUT
  //PORTD &= B00001111;                        // Set level of pins D4-D7 to LOW
  delayMicroseconds(230);                    // Short delay
  int pinvalues;
  for (int pin = 0; pin < 2; pin++) {
    // turn the pin on:
    linePinStates[pin] = digitalRead(linePins[pin]);
    pinvalues = concat(pinvalues,linePinStates[pin]);
  }
  //sscanf(linePinStates, "%d", &pinValues);
//  pins >>= 4;                                // Drop off first four bits of the port; keep only pins D4-D7
  /*for (int pin = 0; pin < 4; pin++) {
    // turn the pin on:
    Serial.print(linePinStates[pin]);
  }*/
  Serial.println(pinvalues);
  //Serial.println(linePinStates[]);                 // Display result of D4-D7 pins in Serial Monitor
  
  // Determine how to steer based on state of the four QTI sensors
  int vL, vR;
  switch(pinvalues)                               // Compare pins to known line following states
  {
    case 11:                             
      vL = -100;                             // -100 to 100 indicate course correction values
      vR = -100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(500);                                // Delay for 50 milliseconds (1/20 second)
      vL = 100;                             // -100 to 100 indicate course correction values
      vR = -100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(1000);                                // Delay for 50 milliseconds (1/20 second)
      break;
    case 10:                        
      vL = -100;                             // -100 to 100 indicate course correction values
      vR = -100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(500);                                // Delay for 50 milliseconds (1/20 second)
      vL = 100;                             // -100 to 100 indicate course correction values
      vR = -100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(1000);                                // Delay for 50 milliseconds (1/20 second)
      break;
    case 01:                        
      vL = -100;                             // -100 to 100 indicate course correction values
      vR = -100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(500);                                // Delay for 50 milliseconds (1/20 second)
      vL = -100;                             // -100 to 100 indicate course correction values
      vR = 100;                              // -100: full reverse; 0=stopped; 100=full forward
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(1000);                                // Delay for 50 milliseconds (1/20 second)
      break;
    default:
      vL = 100;
      vR = 100;
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(50);                                // Delay for 50 milliseconds (1/20 second)
      break;
  }
}
int concat(int x, int y){
    char str1[20];
    char str2[20];

    sprintf(str1,"%d",x);
    sprintf(str2,"%d",y);

    strcat(str1,str2);

    return atoi(str1);
}

