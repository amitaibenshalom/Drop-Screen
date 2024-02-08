void setup() {
  Serial.begin(9600);
}

void loop() {
  for (byte i = 50; i < 256; i++) {
    Serial.write(i);
    delay(1000);
  }
}
