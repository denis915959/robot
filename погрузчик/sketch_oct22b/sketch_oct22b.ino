void setup() { // порт 5
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  analogWrite(8, 60);
  delay(5000);
}