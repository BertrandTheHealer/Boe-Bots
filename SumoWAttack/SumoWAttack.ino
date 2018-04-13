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
const int setpoint = 0;                      // Target distances
const int kpl = -50;                         // Proportional control constants
const int kpr = -50;

// Perform these steps with the Arduino is first powered on
void setup()
{
  pinMode(11, INPUT);  pinMode(10, OUTPUT);   // Left IR LED & Receiver
  pinMode(5, INPUT);  pinMode(4, OUTPUT);    // Right IR LED & Receiver
  
  Serial.begin(9600);                        // Set up Arduino Serial Monitor at 9600 baud
  servoL.attach(13);                         // Attach (programmatically connect) servos to pins on Arduino
  servoR.attach(12);
  servoL.writeMicroseconds(1500);            //stop the motors
  servoR.writeMicroseconds(1500);            //stop the motors
  delay(5000);                               //wait
}

// This code repeats indefinitely
void loop()
{
  for (int pin = 0; pin < 2; pin++) {
    pinMode(linePins[pin], OUTPUT);
    digitalWrite(linePins[pin], HIGH);
  }
  delayMicroseconds(230);                    // Short delay to allow capacitor charge in QTI module
  for (int pin = 0; pin < 2; pin++) {
    // turn the pin on:
    pinMode(linePins[pin], INPUT);
    digitalWrite(linePins[pin], LOW);
  }
  delayMicroseconds(230);                    // Short delay
  int pinvalues;
  for (int pin = 0; pin < 2; pin++) {
    // turn the pin on:
    linePinStates[pin] = digitalRead(linePins[pin]);
    pinvalues = concat(pinvalues,linePinStates[pin]);
  }
  //Serial.println(pinvalues);
  
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
      //Attack code
      int irLeft = irDistance(10, 11);            // Measure left distance
      int irRight = irDistance(4, 5);            // Measure right distance
      Serial.print(irLeft);Serial.print(", ");Serial.println(irRight);


      int vL = (setpoint - irLeft) * kpl;
      int vR = (setpoint - irRight) * kpr;
      Serial.print(vL);Serial.print(", ");Serial.println(vR);
      if ((irLeft == setpoint)&&(irRight == setpoint)) {
        vL = 200;
        vR = 200;
      }
      
      servoL.writeMicroseconds(1500 + vL);      // Steer robot to recenter it over the line
      servoR.writeMicroseconds(1500 - vR);      // 
      delay(20);                                // Delay for 50 milliseconds (1/20 second)
      break;
  }
}

//Functions

//concatenate integers
int concat(int x, int y){
    char str1[20];
    char str2[20];

    sprintf(str1,"%d",x);
    sprintf(str2,"%d",y);

    strcat(str1,str2);

    return atoi(str1);
}

// IR distance measurement function
int irDistance(int irLedPin, int irReceivePin)
{  
  int distance = 0;
  for(long f = 38000; f <= 42000; f += 1000) {
    distance += irDetect(irLedPin, irReceivePin, f);
  }
  return distance;
}

//IR Detection Function
int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}     
