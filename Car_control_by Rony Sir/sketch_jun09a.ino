  
String voice;
int
in1 = 2, //Connect LED 1 To Pin #2
in2 = 3, //Connect LED 2 To Pin #3
in3 = 4, //Connect LED 3 To Pin #4
in4 = 5; //Connect LED 4 To Pin #5

int En1=9;
int En2=10;


//--------------------------Call A Function-------------------------------// 

int i = 0;
int pos;


//-----------------------------------------------------------------------// 
void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(En1,OUTPUT);
  
  pinMode(En2,OUTPUT);
  
  
} 
void loop() {
  while (Serial.available()){  //Check if there is an available byte to read
  delay(10); //Delay added to make thing stable
  char c = Serial.read(); //Conduct a serial read
  if (c == '#') {break;} //Exit the loop when the # is detected after the word
  voice += c; //Shorthand for voice = voice + c
  } 
  if (voice.length() > 0) {
    Serial.println(voice);
//-----------------------------------------------------------------------//   
  //----------Control Multiple Pins/ LEDs----------// 
       if(voice == "up")
    {
    
    digitalWrite(in2,LOW);
      digitalWrite(in4,LOW);
        digitalWrite(in1,HIGH);
          digitalWrite(in3,HIGH);
          analogWrite(En1,255);
          analogWrite(En2,255);
    
    }
   else if(voice == "down")
   {
      digitalWrite(in1,LOW);
      digitalWrite(in3,LOW);
        digitalWrite(in2,HIGH);
          digitalWrite(in4,HIGH);
    analogWrite(En1,255);
          analogWrite(En2,255);
   
   }
   
   else if(voice == "stop")
   {
   digitalWrite(in1,LOW);
      digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
          digitalWrite(in4,LOW);
   
   
    analogWrite(En1,0);
          analogWrite(En2,0);
   
   }
   else if(voice=="Right")
   {
        digitalWrite(in1,LOW);
      digitalWrite(in3,HIGH);
        digitalWrite(in2,LOW);
          digitalWrite(in4,LOW);
         delay(200);
         
          digitalWrite(in1,LOW);
      digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
          digitalWrite(in4,LOW);
           analogWrite(En1,255);
          analogWrite(En2,255);
          
   }
   else if (voice=="Left")
   {
        digitalWrite(in1,HIGH);
      digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
          digitalWrite(in4,LOW);
          delay(200);
          
            digitalWrite(in1,LOW);
      digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
          digitalWrite(in4,LOW);
           analogWrite(En1,255);
          analogWrite(En2,255);
   
   }
     
   
 
voice="";}} //Reset the variable after initiating
