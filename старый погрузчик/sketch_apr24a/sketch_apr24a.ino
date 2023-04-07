void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int i=0, num=0;
while(1)
{
  num=i%3;
  switch(num)
  {
    case 0: case 1:
    num++;
    break;
    case 2:
    num=num;
    break;
  }
  i++;
  Serial.println(num);
  delay(300);
}
}
