void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
while (c==1)//прием сигнала со склада
{
  if( mySwitch2.available() ){

        val = mySwitch2.getReceivedValue();
        if (val>=10){
        Serial.println(val);
        if ((val==10)||(val==11)||(val==12))
{
         // c=0; 
         analogWrite( 6, 0 );//убрать
        delay(10);
          mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
    mySwitch.send(9, 4);//передача сигнала на склад
    delay(10);
mySwitch.send(9, 4);//передача сигнала на склад, возможно 5
    delay(10);
          break;     
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 
servo1.write(rotate_v);
      switch(val){
     case 10://использование сигнала, прибывшего со склада
        delay(1);
        break;
     case 11:
     analogWrite( 6, 140 );
        delay(1000);
     //while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
      //  delay(50);
    //}
        break;   
     case 12:
     analogWrite( 6, 140 );
        delay(1000);
    // while(digitalRead(8)==1)//do
    //{
      //  analogWrite( 6, 140 );
     //   delay(50);
    //}
    break;
    } 
}
