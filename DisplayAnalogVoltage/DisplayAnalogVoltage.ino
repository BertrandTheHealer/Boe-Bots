void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = digitalRead(4);
  //float voltage= sensorValue * (5.0 / 1023.0);
  Serial.println(sensorValue);
}
