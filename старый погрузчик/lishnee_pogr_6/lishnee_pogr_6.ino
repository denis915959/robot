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
          //break; 
          c=0;    
}
        }
        mySwitch2.resetAvailable();
}
}//конец приема сигнала со склада 
}
