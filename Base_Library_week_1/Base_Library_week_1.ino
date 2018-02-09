#include <Servo.h> // Include servo library

//declare Servos
Servo servoLeft;
Servo servoRight;
int serialString = 0;
int runAgain = 114;

void setup()
{
  //Setup servos
  servoRight.attach(13);
  servoLeft.attach(12);
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Ready");

  driveStraight(50, 1);//drive forward at 50% power for 1 second
  turnDegrees(1, 90);//turn right about 90 degrees
  driveStraight(50, 2);//drive forward at 50% power for 2 seconds
  turnDegrees(1, 90);//turn right about 90 degrees
  driveStraight(50, 1);//drive forward at 50% power for 1 second
  turnDegrees(1, 90);//turn right about 90 degrees
  driveStraight(50, 2);//drive forward at 50% power for 2 seconds
  
  driveStraight(-50, 2);//drive backward at 50% power for 2 seconds
  turnDegrees(-1, 90);//turn left about 90 degrees
  driveStraight(-50, 1);//drive backward at 50% power for 1 second
  turnDegrees(-1, 90);//turn left about 90 degrees
  driveStraight(-50, 2);//drive backward at 50% power for 2 seconds
  turnDegrees(-1, 90);//turn left about 90 degrees
  driveStraight(-50, 1);//drive backward at 50% power for 1 second

  Serial.println("Type 'r' to run again");
  //turnDuration(30, 1);//turn right at 30% power for 1 second
}

void loop()
{
  //Servo values: 1500 = stopped, 1700 = full speed forwards, 1300 = full speed backwards

}

void serialEvent(){
  serialString = Serial.read();
  //Serial.println(serialString);
  if(serialString == 114){
    Serial.println(" ");
    driveStraight(50, 1);//drive forward at 50% power for 1 second
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 2);//drive forward at 50% power for 2 seconds
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 1);//drive forward at 50% power for 1 second
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 2);//drive forward at 50% power for 2 seconds
    
    driveStraight(-50, 2);//drive backward at 50% power for 2 seconds
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 1);//drive backward at 50% power for 1 second
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 2);//drive backward at 50% power for 2 seconds
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 1);//drive backward at 50% power for 1 second
  
    Serial.println("Type 'r' to run again");
  }
}

//Function to drive forward. 
//Duration in seconds, velocity in percent of maximum. Negative velocity means backwards.
void driveStraight(int velocity, int duration){
  Serial.print("Driving at ");
  Serial.print(velocity);
  Serial.print("% power for ");
  Serial.print(duration);
  Serial.println(" seconds");
  //if duration is longer than a minute, reduce it to a minute
  if (duration > 60)
      {
        duration = 60;
        Serial.println("Durations greater than 1 minute not allowed");
      }

  //if the speed is within proper bounds
  if (velocity <= 100)
  {
    if (velocity >= -100)
    {
      //set both servos to correct speed
      servoLeft.writeMicroseconds(1500+(velocity*2));
      servoRight.writeMicroseconds(1500-(velocity*2));
      //leave them that way for the specified amount of time
      delay(duration*1000);
      //stop them
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
    }
  }
  else {
    Serial.println("Velocity out of bounds. Use a number between -100 and 100");
  }
}

//Function to turn based on speed and time. 
//Duration in seconds, velocity in percent of maximum. Negative velocity means left, positive right.
void turnDuration(int velocity, int duration){
  Serial.print("Turning at ");
  Serial.print(velocity);
  Serial.print("% power for ");
  Serial.print(duration);
  Serial.println(" seconds");
  //if duration is longer than a minute, reduce it to a minute
  if (duration > 60)
      {
        duration = 60;
        Serial.println("Durations greater than 1 minute not allowed");
      }

  //if the speed is within proper bounds
  if (velocity <= 100)
  {
    if (velocity >= -100)
    {
      //set both servos to correct speed
      servoLeft.writeMicroseconds(1500+(velocity*2));
      servoRight.writeMicroseconds(1500+(velocity*2));
      //leave them that way for the specified amount of time
      delay(duration*1000);
      //stop them
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
    }
  }
  else {
    Serial.println("Velocity out of bounds. Use a number between -100 and 100");
  }
}

//Function to turn set number of degrees. Only approximate 
//Direction; Negative # for left, positive # for right. Angle in degrees.
void turnDegrees(int wdirection, int angle){
  //if angle is greater than two full turns, reduce it to two full turns
  if (angle > 720)
      {
        angle = 720;
        Serial.println("Angles greater than 720 degrees not allowed");
      }
  angle = (int) angle;//make sure angle is an integer
  //if the speed is within proper bounds
  if (wdirection >= 0)
  {
    Serial.print("Turning Right ");
    Serial.print(angle);
    Serial.println(" degrees");
    //set both servos to correct speed
    servoLeft.writeMicroseconds(1560);
    servoRight.writeMicroseconds(1560);
    //leave them running for the appropriate amount of time
    delay(angle*8);
  }
  if(wdirection < 0)
  {
    Serial.print("Turning Left ");
    Serial.print(angle);
    Serial.println(" degrees");
    //set both servos to correct speed
    servoLeft.writeMicroseconds(1440);
    servoRight.writeMicroseconds(1440);
    //leave them running for the appropriate amount of time
    delay(angle*7);
  }
  //stop them
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
}
