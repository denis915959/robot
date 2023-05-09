void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(32, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  while (digitalRead(32)==1)
  {
    digitalWrite(12, HIGH); //все верно выставлено для движения вниз!!!!!!!!!!!!!!!!!!!
    digitalWrite(13, LOW);
    delay(50);
  }
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(5000);
}
