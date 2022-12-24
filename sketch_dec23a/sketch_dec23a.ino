#define BLYNK_PRINT Serial
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
const int PulseWire = A0; 
const int tempPin = A2;
int UpperThreshold = 518;
int LowerThreshold = 490;
WidgetLED led(V4);
int val;
int Threshold = 550;
PulseSensorPlayground pulseSensor;
Adafruit_MPU6050 mpu;
#define EspSerial Serial
char auth[] = "uM55RgOaKPE8rHFGMuDg40FwiOLCVY3l";                                                      
char ssid[] = "DELTA";    
char pass[] = "asdf4321@roy";  
    //// 19,18
#define ESP8266_BAUD 9600
ESP8266 wifi(&EspSerial); 






void setup() {

  Serial.begin(9600);



  
EspSerial.begin(ESP8266_BAUD);
Blynk.begin(auth, wifi, ssid, pass, "192.168.0.103", 8080);
  pulseSensor.analogInput(PulseWire);   
     
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
   Serial.print("sfdadsa");

}

  //For mpu
  pinMode(4, INPUT);
  pinMode(5, INPUT);

    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1)
    {
      delay(10);}
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  //End mpu
delay(100);
  
}
int i = 100;
void loop() {
  val = analogRead(tempPin);
  float mv = ( val/1024.0)*5000;
  float cel = mv/10;
  float farh = (cel*9)/5 + 32;
  Blynk.virtualWrite(V1, farh); 
  int myBPM = pulseSensor.getBeatsPerMinute(); 
if(i>0)
  {
    Blynk.virtualWrite(V0, i); 
    }
    i-- ;
  if (pulseSensor.sawStartOfBeat()) {       
         
    Blynk.virtualWrite(V0, myBPM);                        
    }
 
   sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  if(a.acceleration.x <2)
  {
    led.on();
    }
    
  else{
    led.off();
    }
    
   

 delay(20);


  
}
    
