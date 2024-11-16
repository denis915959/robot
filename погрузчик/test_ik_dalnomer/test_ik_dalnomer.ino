// аналоговый пин для подключения выхода Vo сенсора
const int IRpin = A0;
float value; // для хранения аналогового значения

void setup() {
  Serial.begin(9600);
  Serial.println("start");
}

void loop() {
// получаем сглаженное значение и переводим в напряжение
  value = irRead(); //medium_read();
  Serial.println(value);
  Serial.println();
  delay(1000);
}

// Усреднение нескольких значений для сглаживания
float irRead() {
  float value;
  int averaging = 0; // переменная для суммирования данных
  int cycl = 3;  // 6
  int delay_const = 0; // 15
  float k = 1.1;
  // Получение 5 значений
  int analog_sum = 0;
  float volts_sum = 0;
  for (int i=0; i<cycl; i++)
  {
    value = analogRead(IRpin);
    analog_sum+=value;

    // значение сенсора переводим в напряжение
    float volts = analogRead(IRpin)*0.0048828125;
    volts_sum+=volts;
    // и в расстояние в см
    int distance=32*pow(volts,-1.10);
    averaging = averaging + distance;
    //delay(delay_const); // Ожидание 55 ms перед каждым чтением
  }
  float result = -1.0;
  value = averaging / cycl; // усреднить значения
  Serial.print("analog = ");
  Serial.println(analog_sum/cycl);
  Serial.print("volts = ");
  Serial.println(volts_sum/cycl);
  if(value>80){
    value = -1;
  }
  else{
    result = value/k;
  }
  
  return(result);
}

float medium_read(){
  int  d1 = irRead();
  delay(50);
  int  d2 = irRead();
  delay(50);
  int  d3 = irRead();
  delay(50);
  int  d4 = irRead();
  delay(50);
  int  d5 = irRead();
  delay(50);
  float res = (d1 + d2 + d3 + d4 + d5)/5;
  return(res);
}
