#include <Ps3Controller.h>

//Right motor
int enableRightMotor=12; 
int rightMotorPin1=15;
//Left motor
int enableLeftMotor=13;
int leftMotorPin1=14;

int buzzerPin=5;
int lightPin=4;
bool lightStatus=false;

int MAX_SPEED=255;
int MIN_SPEED=0;

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed){
  if (rightMotorSpeed < 0){
    digitalWrite(rightMotorPin1,LOW);
  }
  else if (rightMotorSpeed > 0){
    digitalWrite(rightMotorPin1,HIGH);
  }
  else{
    digitalWrite(rightMotorPin1,LOW);
  }
  
  if (leftMotorSpeed < 0){
    digitalWrite(leftMotorPin1,LOW);
  }
  else if (leftMotorSpeed > 0){
    digitalWrite(leftMotorPin1,HIGH);
  }
  else{
    digitalWrite(leftMotorPin1,LOW);
  } 

  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));   
}


void notify(){
  int yAxisValue =(Ps3.data.analog.stick.ly);  //Left stick  - y axis - forward/backward car movement
  int xAxisValue =(Ps3.data.analog.stick.rx);  //Right stick - x axis - left/right car movement


  int throttle = map( yAxisValue, 127, -128, -255, 255);
  int steering = map( xAxisValue, -128, 127, -255, 255);  

  
  if(throttle>50){
    if(steering>50){
      digitalWrite(rightMotorPin1,HIGH);
      digitalWrite(leftMotorPin1,HIGH);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle)-abs(steering*0.85));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)); 
    } else if(steering<-50){
      digitalWrite(rightMotorPin1,HIGH);
      digitalWrite(leftMotorPin1,HIGH);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)-abs(steering*0.85)); 
    } else {
      Serial.print("F: ");
      Serial.println(throttle);
      digitalWrite(rightMotorPin1,HIGH);
      digitalWrite(leftMotorPin1,HIGH);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)); 
    }
  } else if(throttle<-50){
    if(steering>50){
      digitalWrite(rightMotorPin1,LOW);
      digitalWrite(leftMotorPin1,LOW);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)-abs(steering*0.85)); 
    } else if(steering<-50){
      digitalWrite(rightMotorPin1,LOW);
      digitalWrite(leftMotorPin1,LOW);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle)-abs(steering*0.85));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)); 
    } else {
      Serial.print("B: ");
      Serial.println(throttle);
      digitalWrite(rightMotorPin1,LOW);
      digitalWrite(leftMotorPin1,LOW);
      ledcWrite(rightMotorPWMSpeedChannel, abs(throttle));
      ledcWrite(leftMotorPWMSpeedChannel, abs(throttle)); 
    }
  } else if (steering>50){
    Serial.print("R: ");
    Serial.println(steering);
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(leftMotorPin1,LOW);
    ledcWrite(rightMotorPWMSpeedChannel, abs(steering));
    ledcWrite(leftMotorPWMSpeedChannel, abs(steering)); 
  } else if (steering<-50){
    Serial.print("L: ");
    Serial.println(steering);
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(leftMotorPin1,HIGH);
    ledcWrite(rightMotorPWMSpeedChannel, abs(steering));
    ledcWrite(leftMotorPWMSpeedChannel, abs(steering)); 
  } else if ((throttle>-50 && throttle<50) || (steering>-50 && steering<50)){
        ledcWrite(rightMotorPWMSpeedChannel, MIN_SPEED);
        ledcWrite(leftMotorPWMSpeedChannel, MIN_SPEED); 
  }


  if(Ps3.event.button_down.cross){
    digitalWrite(buzzerPin,HIGH);
    delay(10);
  } else if (Ps3.event.button_up.cross){
    digitalWrite(buzzerPin,LOW);
  }

  if(Ps3.event.button_down.cross){
    digitalWrite(buzzerPin,HIGH);
    delay(10);
  } 
  else if(Ps3.event.button_up.cross){
    digitalWrite(buzzerPin,LOW);
  }

  if(Ps3.event.button_down.circle){
    lightStatus=!lightStatus;
    digitalWrite(lightPin,lightStatus);
  }
}

void onConnect(){
  Serial.println("Connected!.");
}

void onDisConnect(){
  rotateMotor(0, 0);
  Serial.println("Disconnected!.");    
}

void setUpPinModes()
{
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);

  pinMode(buzzerPin,OUTPUT);
  pinMode(lightPin,OUTPUT);

  //Set up PWM for motor speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);  
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel);  
  
}


void setup(){
  setUpPinModes();
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisConnect);
  Ps3.begin("78:21:84:7e:45:3a");       // The MAC address of ESP32 stored in the PS3 Controller.
  Serial.println("Ready.");
}

void loop(){

}
