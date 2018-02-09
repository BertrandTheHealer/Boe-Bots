/*
 * Functionality:
 * -Use serial monitor input to move, turn, and wait, either remote-control style or program
 *  style
 * -Enable and disable verbose feedback from serial monitor
 * -Run demo program from serial monitor
 * -Complete documentation in serial monitor
 * -Run short movement on reset; can be enabled and disabled through serial monitor
 * -Remembers verbosity and initial movement settings between restarts
 * Created by Max Davy 02/08/18
*/
#include <Servo.h> // Include servo library
#include <EEPROM.h> // Include EEPROM library

//declare Servos
Servo servoLeft;
Servo servoRight;

//declare variables
bool verbosity = false;
int serialString = 0;
byte value = 1;//for EEPROM
const int PROGMEM startLoc = 1;//location in memory to read initial movement from
const int PROGMEM verbLoc = 5;//location in memory to read verbosity from

//declare key values
const int PROGMEM r = 114;
const int PROGMEM a = 97;
const int PROGMEM d = 100;
const int PROGMEM w = 119;
const int PROGMEM s = 115;
const int PROGMEM t = 116;
const int PROGMEM h = 104;
const int PROGMEM v = 118;
const int PROGMEM i = 105;
//PROGMEM places the variable in flash memory


void setup()
{
  //Setup servos
  servoRight.attach(13);
  servoLeft.attach(12);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);

   //start serial connection and print instructions
  Serial.begin(9600);
  Serial.println("Ready");
  printHelp();

  //the idea behind this part is that when you press the reset button it does a new movement.
  //so it keeps track of what movement and whether to make a movement using eeprom, which is 
  //a form of persistent memory. a value of 12 means do nothing, whereas values of 1,2,3,and 4
  //means go forward, backward, right, and left respectively. After the bot makes a movement,
  //it writes which movement to do next into the eeprom to be executed next time.

  //read verbosity from EEPROM
  EEPROM.get(verbLoc, verbosity);
  if(verbosity){
    Serial.println("Verbose Output Enabled");
  }else{
    Serial.println("Verbose Output Disabled");
  }

  //Read initial movement from EEPROM
  EEPROM.get(startLoc, value);
  if(verbosity){Serial.print("Movement: ");Serial.println(value);}
  if(value==12){//if value is 12
    Serial.println("Initial movement off");
    //movement disabled, do nothing
  }else if(value==2){//if value is 2
    driveStraight(-50, 1000);//drive backward
    value = 3;//set value to 3
    EEPROM.put(startLoc, value);//and write to eeprom
  }else if(value==3){//if value is 3
    turnDegrees(1, 60);//turn right
    value = 4;//set value to 4
    EEPROM.put(startLoc, value);//and write to eeprom
  }else if(value==4){//if value is 4
    turnDegrees(-1, 60);//turn left
    value = 1;//set value to 1
    EEPROM.put(startLoc, value);//and write to eeprom
  }else{//if value is 0
    driveStraight(50, 1000);//drive forward
    value = 2;//set value to 2
    EEPROM.put(startLoc, value);//and write to eeprom
  }
  //turnDuration(30, 1000);//turn right at 30% power for 1 second
}

void loop()
{
  //Servo values: 1500 = stopped, 1700 = full speed forwards, 1300 = full speed backwards

}

void printHelp()
{
  Serial.println("\nType 'r' to run demo code");
  Serial.println("Use 'w' 'a' 's' and 'd' to navigate");
  Serial.println("Use 'W' 'A' 'S' and 'D' to navigate in larger increments");
  Serial.println("Use 't' to wait .5 second; use 'T' to wait 1 second");
  Serial.println("Use 'v' to enable/disable verbose output");
  Serial.println("Use 'i' to enable/disable initial movement");
  Serial.println("Commands can be concatenated (e.g. 'ttddTww')");
  Serial.println("Type 'h' to show this message again");
}

void serialEvent(){//when something is typed into the serial monitor
  serialString = Serial.read();//read the input
  if(verbosity && (serialString != 10) && (serialString != 13)){Serial.print("\nKeycode:");Serial.println(serialString);}
  if(serialString == r){//if the input is 'r'
    Serial.println(" ");
    Serial.println("Demo Starting");
    driveStraight(50, 1000);//drive forward at 50% power for 1 second
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 2000);//drive forward at 50% power for 2 seconds
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 1000);//drive forward at 50% power for 1 second
    turnDegrees(1, 90);//turn right about 90 degrees
    driveStraight(50, 2000);//drive forward at 50% power for 2 seconds
    
    driveStraight(-50, 2000);//drive backward at 50% power for 2 seconds
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 1000);//drive backward at 50% power for 1 second
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 2000);//drive backward at 50% power for 2 seconds
    turnDegrees(-1, 90);//turn left about 90 degrees
    driveStraight(-50, 1000);//drive backward at 50% power for 1 second
  
    Serial.println("Type 'r' to run again");
  }
  else if(serialString == w){//w
    Serial.println(" ");
    Serial.println("Forwards");
    driveStraight(50, 200);//drive forward at 50% power for .2 second
  }
  else if(serialString == (w-32)){//capital w
    Serial.println(" ");
    Serial.println("Forwards Farther");
    driveStraight(50, 1000);//drive forward at 50% power for 1 second
  }
  else if(serialString == s){//s
    Serial.println(" ");
    Serial.println("Backwards");
    driveStraight(-50, 200);//drive backward at 50% power for .2 second
  }
  else if(serialString == (s-32)){//capital s
    Serial.println(" ");
    Serial.println("Backwards Farther");
    driveStraight(-50, 1000);//drive backward at 50% power for 1 second
  }
  else if(serialString == a){//a
    Serial.println(" ");
    Serial.println("Left");
    turnDegrees(-1, 20);//turn 10 degrees left
  }
  else if(serialString == (a-32)){//capital a
    Serial.println(" ");
    Serial.println("Left Farther");
    turnDegrees(-1, 60);//turn 60 degrees left
  }
  else if(serialString == d){//d
    Serial.println(" ");
    Serial.println("Right");
    turnDegrees(1, 20);//turn 10 degrees right
  }
  else if(serialString == (d-32)){//capital d
    Serial.println(" ");
    Serial.println("Right Farther");
    turnDegrees(1, 60);//turn 60 degrees right
  }
  else if(serialString == t){//t
    Serial.println(" ");
    Serial.println("Waiting .5s...");
    delay(500);
  }
  else if(serialString == (t-32)){//capital t
    Serial.println(" ");
    Serial.println("Waiting 1s...");
    delay(1000);
  }
  else if(serialString == h){
    printHelp();
  }
  else if(serialString == v){
    EEPROM.get(verbLoc, verbosity);
    if(verbosity){
      verbosity=false;
      EEPROM.put(verbLoc, verbosity);
      Serial.println("\nVerbose Feedback Off");
      }
    else{
      verbosity=true;
      EEPROM.put(verbLoc, verbosity);
      Serial.println("\nVerbose Feedback On");
      }
  }
  else if(serialString == i){//i
    EEPROM.get(startLoc, value);//read from eeprom
    if(value==12){//if value is 12 (aka initial movement disabled)
      value=1;//set the value to 1 (enable initial movement)
      EEPROM.put(startLoc, value);//and save it to eeprom
      Serial.println("\nInitial Movement On");
      }
    else{//if value isn't 12 (aka initial movement  enabled)
      value=12;//set the value to 1 (disable initial movement)
      EEPROM.put(startLoc, value);//and write to eeprom
      Serial.println("\nInitial Movement Off");
      }
  }
}

//Function to drive forward. 
//Duration in milliseconds, velocity in percent of maximum. Negative velocity means backwards.
void driveStraight(int velocity, int duration){
  if(verbosity){
    Serial.print("Driving at ");
    Serial.print(velocity);
    Serial.print("% power for ");
    Serial.print(duration);
    Serial.println(" milliseconds");
  }
  //if duration is longer than a minute, reduce it to a minute
  if (duration > 60000)
      {
        duration = 60000;
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
      delay(duration);
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
//Duration in milliseconds, velocity in percent of maximum. Negative velocity means left, positive right.
void turnDuration(int velocity, int duration){
  if(verbosity){
    Serial.print("Turning at ");
    Serial.print(velocity);
    Serial.print("% power for ");
    Serial.print(duration);
    Serial.println(" milliseconds");
  }
  //if duration is longer than a minute, reduce it to a minute
  if (duration > 60000)
      {
        duration = 60000;
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
      delay(duration);
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
    if(verbosity){
      Serial.print(angle);
      Serial.println(" degrees");
    }
    //set both servos to correct speed
    servoLeft.writeMicroseconds(1560);
    servoRight.writeMicroseconds(1560);
    //leave them running for the appropriate amount of time
    delay(angle*8);
  }
  if(wdirection < 0)
  {
    Serial.print("Turning Left ");
    if(verbosity){
      Serial.print(angle);
      Serial.println(" degrees");
    }
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
