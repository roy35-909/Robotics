#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <SPI.h>
#include <MFRC522.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#define WIFI_SSID "DELTA"
#define WIFI_PASSWORD "asdf4321@roy"
#define API_KEY "AIzaSyAm9AdSk9zASEZjnMR-5rZXkoPihrSmeok"
#define FIREBASE_PROJECT_ID "project-pi-b18db"
#define USER_EMAIL "roy35-909@diu.edu.bd"
#define USER_PASSWORD "123456789"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
constexpr uint8_t RST_PIN = D2; 
constexpr uint8_t SS_PIN = D4; 
MFRC522 mfrc522(SS_PIN, RST_PIN);
String classcode; 
// For Time 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"asia.pool.ntp.org",21600,6000);

void fcsUploadCallback(CFS_UploadStatusInfo info)
{
    if (info.status == fb_esp_cfs_upload_status_init)
    {
        Serial.printf("\nUploading data (%d)...\n", info.size);
    }
    else if (info.status == fb_esp_cfs_upload_status_upload)
    {
        Serial.printf("Uploaded %d%s\n", (int)info.progress, "%");
    }
    else if (info.status == fb_esp_cfs_upload_status_complete)
    {
        Serial.println("Upload completed ");
    }
    else if (info.status == fb_esp_cfs_upload_status_process_response)
    {
        Serial.print("Processing the response... ");
    }
    else if (info.status == fb_esp_cfs_upload_status_error)
    {
        Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
    }
}


String day[]={" Sunday"," Monday"," Tuesday"," Wednesday"," Thursday", " Friday", " Saturday"};

void setup()
{

    Serial.begin(115200);
     pinMode(D3,OUTPUT);
     digitalWrite(D3,LOW);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    config.api_key = API_KEY;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;
    config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

    Firebase.begin(&config, &auth);

    Firebase.reconnectWiFi(true);
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
 
  Serial.println(F("Read personal data on a MIFARE PICC:")); 

timeClient.begin();
    
}

void loop()
{
        
        timeClient.update();
        FirebaseJson content;
       
                                         //we need to create a collection in firebase and named past it here .
          

        String Date = (timeClient.getFormattedTime())+day[timeClient.getDay()];

        String doc_path = "projects/";
        doc_path += FIREBASE_PROJECT_ID;
        doc_path += "/databases/(default)/documents/coll_id/doc_id";      
        
        
        
          
// Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  char str[32] = "";
   array_to_string( mfrc522.uid.uidByte, 4, str); //Insert (byte array, length, char array for output)
   Serial.println(str);
   Serial.println(F("\n**End Reading**\n"));

        String documentPath ; 
        String mood;
        Serial.print("Get a document... ");
        documentPath = "mood/status";
        String mask = "";
        if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(),mask.c_str())){
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            FirebaseJson payload;
            payload.setJsonData(fbdo.payload().c_str());

            // Get the data from FirebaseJson object 
            FirebaseJsonData jsonData;
            payload.get(jsonData, "fields/status/stringValue", true);
            Serial.println(jsonData.stringValue);
            mood = jsonData.stringValue;
            if(jsonData.stringValue == "food"){
              documentPath = "foods/";
              
              }
              else if(jsonData.stringValue == "coffee"){
                
                documentPath = "coffee/";
                }
              else if(jsonData.stringValue == "attendance"){
                
                documentPath = "attendance/";
                }
              else if(jsonData.stringValue=="umbrella"){
                
                documentPath = "umbrella/";
                
                }
             
        }
        else{
            Serial.println(fbdo.errorReason());}

   

  if(mood=="attendance"){
     content.set("fields/atten_date_time/stringValue", Date);
     content.set("fields/atten_id/stringValue", str);
    
    
    }
  else if(mood=="food"){
    
        content.set("fields/get_food_date/stringValue", Date);
        content.set("fields/get_food_id/stringValue", str);
    }
  else if(mood=="coffee"){
    
            content.set("fields/take_coffee_date/stringValue", Date);
            content.set("fields/take_coffee_id/stringValue", str);
    }
   else if(mood=="umbrella"){
            content.set("fields/status/stringValue", "Not Return");
            content.set("fields/take_um_id/stringValue", str);
    
    
    }
   else if(mood=="stop"){
    
    return ;
    }
 

       

        Serial.print("Create a document... ");

        if (Firebase.Firestore.createDocument(&fbdo, FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, documentPath.c_str(), content.raw()))
            Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
            if(mood=="coffee"){
              digitalWrite(D3,HIGH);
              delay(5000);
               digitalWrite(D3,LOW);
              
              }
        else
            Serial.println(fbdo.errorReason());





            
            delay(200);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
    
}


void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}
