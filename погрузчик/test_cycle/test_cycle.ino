void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
for (int i=2; i<6; i++)
{
  pinMode(i, OUTPUT);
}
for (int i=2; i<6; i++)
{
  digitalWrite(i, HIGH);
}
delay(3000);
for (int i=2; i<6; i++)
{
  digitalWrite(i, LOW);
}
delay(3000);
}
