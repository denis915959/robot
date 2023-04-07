void setup() {
  // put your setup code here, to run once:
pinMode(13, INPUT);
pinMode(12, INPUT);
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int i;
  int d;
  i=digitalRead(13);
  d=digitalRead(12);
  digitalWrite(7, HIGH);
  digitalWrite(6, LOW);
  analogWrite(5, 255);
  if (d==1)
  {
    analogWrite(5, 0);
    delay(2000);
    digitalWrite(6, HIGH);
    digitalWrite(7, LOW);
    analogWrite(5, 255);
    delay(10000);
  }
//Serial.println(digitalRead(12));
   //delay(200);
}
