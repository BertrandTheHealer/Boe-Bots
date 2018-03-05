void setup() {
  // put your setup code here, to run once:
  pinMode(10, INPUT);//right line sensor
  pinMode(9, INPUT);//left line sensor
  Serial.begin(9600);// Set data rate to 9600 bps
}

void loop() {

  //Serial.print(analogRead(9));                      // Display left distance
  Serial.print("  ");                        // Display spaces
  Serial.println(digitalRead(10));                   // Display right distance

  delay(100);                                // 0.1 second delay
}
