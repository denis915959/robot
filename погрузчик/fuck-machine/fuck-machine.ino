void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(9, HIGH);
    delay(300000);
    digitalWrite(9, LOW); // отключено
    delay(90000);
}
