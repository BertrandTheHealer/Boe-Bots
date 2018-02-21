/*
 * Robotics with the BOE Shield - StartResetIndicator
 * Test the piezospeaker circuit.
 */

void setup()                                 // Built in initialization block
{
  Serial.begin(9600);
  Serial.println("Beep!");  

  playtone(4,3000,500);
  playtone(4,2000,500);
  playtone(4,4000,500);
  playtone(4,3000,500);
}  
 
void loop()                                  // Main loop auto-repeats
{         
  Serial.println("Waiting for reset...");
  delay(1000);
}

void playtone(int port, int kHz, int ms){
  tone(port, kHz, ms);                       
  delay(ms); 
}

