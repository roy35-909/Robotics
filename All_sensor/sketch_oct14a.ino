// all
#include <LiquidCrystal.h>
const int rs =2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#include <dht11.h>
#define DHT11PIN A0
// flem fire


//co2_mq135
#define sensor A1
#define buzz 4
#define red 6
#define green 5

int gas, co2lvl;



dht11 DHT11;
int smokeA4 = A4;
// Your threshold value
int sensorThres = 700;

 
 
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Air Quality");
  setCursor(1,1);
  lcd.print("Monitor System");
  delay(1000);
  pinMode(smokeA4, INPUT);
  

   pinMode(LED, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(sensor, INPUT);
}

void loop()
{
  Serial.println();
  lcd.clear();
dht();
mq6();
co2();
}


void mq6(){
  int analogSensor = analogRead(smokeA4);

  Serial.print("Pin A0: ");
  
 lcd.setCursor(0, 0);
 lcd.print("Smoke:");
 lcd.setCursor(6, 0);
 lcd.print(analogSensor);
 lcd.print("ppm");
 
  Serial.println(analogSensor);
  Blynk.virtualWrite(V1, analogSensor);
}

void dht(){
  Serial.println();

  int chk = DHT11.read(DHT11PIN);

  Blynk.virtualWrite(V2, DHT11.humidity);
  Blynk.virtualWrite(V3, DHT11.temperature);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);
  delay(500);
   Serial.print("Pin A0: ");
 lcd.setCursor(0, 1);
 lcd.print("H=");
 lcd.setCursor(2, 1);
 lcd.print((float)DHT11.humidity, 2);
 lcd.print("%");
 lcd.setCursor(8, 1);
 lcd.print("T=");
 lcd.setCursor(10, 1);
 lcd.print((float)DHT11.temperature, 2);
  lcd.print("c*");
 delay(100);
}




void co2() {

  gas = analogRead(sensor);
  co2lvl = gas - 120;
  co2lvl = map(co2lvl, 0, 1024, 400, 5000);
  Serial.println(co2lvl);
  Blynk.virtualWrite(V6, co2lvl);
  lcd.setCursor (0, 3);
  lcd.print("CO2=");
  lcd.setCursor(4, 3);
  lcd.print(co2lvl);
  lcd.print("ppm");

  

  if ((co2lvl >= 350) && (co2lvl <= 1400))
  {
    //lcd.print("  Good ");
    delay(500);
    digitalWrite(buzz, LOW);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
  }

  else if ((co2lvl >= 1400) && (co2lvl <= 2000))
  {
    digitalWrite(buzz, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    //lcd.print("  Bad ");
    delay(1000);

  }
  else
  {
    //lcd.print(" Danger!");
    delay(500);
    digitalWrite(buzz, HIGH);
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
  }
  delay(300);
  //lcd.clear();
}
