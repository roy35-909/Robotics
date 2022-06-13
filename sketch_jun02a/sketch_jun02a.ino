#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN 7
#define DHTTYPE DHT11
const int flamePin = 10;
int gaspin = A0;
int Flame = HIGH;
int security = 12;
int MaxGasValue = 170;
float MaxTempvalue = 33;
int Fan = 2;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() 
{
  pinMode(flamePin, INPUT);
  pinMode(gaspin, INPUT);
  pinMode(security, OUTPUT);
  pinMode(Fan, OUTPUT);
  dht.begin();
  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight and print a message.
  lcd.backlight();
 lcd.print("    Wellcome");
 delay(1000);
 lcd.clear();
}
void Security(bool f=false)
{
  if(f==false){
    digitalWrite(security,LOW);
    
    }
  else{
    digitalWrite(security,HIGH);
    }
}

String Humadity(int x)
{
 if(x>25)
 return "Poor";
 if(x>25&&x<30)
 return "Fair";
 if(x>30&&x<60)
 return "Good";
 if(x>60&&x<70)
 return "Fair";
 if(x>70)
 return "Poor";
 }
void loop() 
{
  

  int gasvalue = analogRead(gaspin);
  int t = dht.readTemperature();
  int h = dht.readHumidity();
  Flame = digitalRead(flamePin);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T = ");
  lcd.print(t);
  lcd.print("\337");
  lcd.print("  H =");
  lcd.print(h);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Gas=");
  lcd.print(gasvalue);
  lcd.print("   ");
  lcd.print(Humadity(h));
  if (Flame== LOW)
  {
  
    Security(true);
    lcd.clear();
    lcd.print(" Fire Detected!");
    digitalWrite(Fan,HIGH);
    delay(2000);
  }
  else{
    
    Security(false);
    digitalWrite(Fan,LOW);
    
    }
 
if(gasvalue>=MaxGasValue){

  Security(true);
  digitalWrite(Fan,HIGH);
  
    lcd.clear();
    lcd.print("  Gas Detected!");
    delay(1000);
  }
else{
  Security(false);
   digitalWrite(Fan,LOW);
  }


if(t>MaxTempvalue){

  digitalWrite(Fan,HIGH);
  delay(1000);
   
}
else{
  digitalWrite(Fan,LOW);
  }

    
  delay(500);
}
