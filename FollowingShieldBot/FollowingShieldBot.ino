/*
 * Robotics with the BOE Shield - FollowingShieldBot
 * Use proportional control to maintain a fixed distance between
 * BOE Shield-Bot and object in front of it.
 */

#include <Servo.h>                           // Include servo library
 
Servo servoL;                             // Declare left and right servos
Servo servoR;

const int setpoint = 0;                      // Target distances
const int kpl = -50;                         // Proportional control constants
const int kpr = -50;
 
void setup()                                 // Built-in initialization block
{
  pinMode(11, INPUT);  pinMode(10, OUTPUT);   // Left IR LED & Receiver
  pinMode(5, INPUT);  pinMode(4, OUTPUT);    // Right IR LED & Receiver

  servoL.attach(13);                      // Attach left signal to pin 13
  servoR.attach(12);                     // Attach right signal to pin 12
  Serial.begin(9600);
}  
 
void loop()                                  // Main loop auto-repeats
{
  int irLeft = irDistance(10, 11);            // Measure left distance
  int irRight = irDistance(4, 5);            // Measure right distance
 
  // Left and right proportional control calculations
  int driveLeft = (setpoint - irLeft) * kpl;     
  int driveRight = (setpoint - irRight) * kpr;
  if ((irLeft == setpoint)&&(irRight == setpoint)) {
    driveLeft = 200;
    driveRight = 200;
  }

  maneuver(driveLeft, driveRight, 20);       // Drive levels set speeds
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

// IR Detection function

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency, 8);              // IRLED 38 kHz for at least 1 ms
  delay(1);                                  // Wait 1 ms
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 1 no detect, 0 detect
}     

void maneuver(int speedLeft, int speedRight, int msTime)
{
  // speedLeft, speedRight ranges: Backward  Linear  Stop  Linear   Forward
  //                               -200      -100......0......100       200
  servoL.writeMicroseconds(1500 + speedLeft);   // Set left servo speed
  servoR.writeMicroseconds(1500 - speedRight); // Set right servo speed
  if(msTime==-1)                                   // if msTime = -1
  {                                  
    servoL.detach();                            // Stop servo signals
    servoR.detach();   
  }
  delay(msTime);                                   // Delay for msTime
}

