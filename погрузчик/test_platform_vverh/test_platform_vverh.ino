void setup() {
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(33, INPUT); //верхний концевик
}
int tmp_verhny_floor=5000; //с новой высотой стеллажей на 12 вольтах 5000 ок                                        //6500 - оптимально на 12 вольтах;  6200 - есть риск, что слишком низко при низком напряжении . 6700 - на 12 вольтах ниже опускать точно не надо (может задевать держателем (квадратом) вилы за стеллаж)
void loop() {
  // put your main code here, to run repeatedly:

  while (digitalRead(33)==1)
  {
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    delay(50);
  }
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(1000);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  delay(tmp_verhny_floor);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  delay(4000);
}
