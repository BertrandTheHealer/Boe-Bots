void setup() {
  // put your setup code here, to run once:
  pinMode(4, INPUT);//right line sensor
  pinMode(5, INPUT);//left line sensor
  tone(9, 3000, 1000);// Play tone for 1 second
  delay(1000);// Delay to finish tone
  Serial.begin(9600);// Set data rate to 9600 bps
}

void loop() {

  Serial.print(digitalRead(5));                      // Display left distance
  Serial.print("  ");                        // Display spaces
  Serial.println(digitalRead(4));                   // Display right distance

  delay(100);                                // 0.1 second delay
}
