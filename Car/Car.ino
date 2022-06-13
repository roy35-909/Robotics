

int motor1pin1 = 2;
int motor1pin2 = 3;
int En1 = 9;
int En2 = 10;
int motor2pin1 = 4;
int motor2pin2 = 5;
unsigned long long int Runtime;

void setup()
{
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(En1,OUTPUT);
  pinMode(En2,OUTPUT);
  Serial.begin(9600);
  
}
void Stop(){
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
  
  }
void RunMotor1()
{
  Stop();
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  
}
void BackwordRunMotor1()
{
  
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
  
}
void RunMotor2()
{
  Stop();
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  
}
void BackwordRunMotor2()
{
  
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  
}

void Forward(){
  analogWrite(En1,255);
  analogWrite(En2,255);
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin2, LOW);
  
  
  }
void Backword(){
  Stop();
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  analogWrite(En1,255);
  analogWrite(En2,255);
  }

void loop()
{
  Stop();
if(Serial.available()){
  delay(10);
char c = Serial.read();
if(c=='F'){
  Forward();
  }
if(c=='B'){
  Backword();
  
  }
if (c=='L'){
  Stop();
  RunMotor1();
  
  }
if(c=='R'){
  Stop();
  RunMotor2();
  
  }
 if(c=='S'){
  Stop();
  }
  
  }

  delay(100);
}
