void setup() {
  // put your setup code here, to run once:
pinMode(7, OUTPUT);
pinMode(6, OUTPUT);
pinMode(5, OUTPUT);
pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int i;
i=analogRead(A1);
while (i>55)
{
  i=analogRead(A1);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  analogWrite(5, 255);
  delay(50);
} 
analogWrite(5, 0);
delay(3000);
i=analogRead(A1);
while (i<620)
{
  i=analogRead(A1);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  analogWrite(5, 255);
  delay(50);
} 
analogWrite(5, 0);
delay(3000);
}
