#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11
int genderBytes=23;//the response includes this number of bytes
byte gazeGen[] = {0xFE, 0x03, 0x03, (byte)0x00, 0x64, (byte)0x00, (byte)0x00,};
byte CameraDown[]= {0xFE, 0x01, 0x01, (byte)0x00, 0x01};//90 Degrees Lens is on bottom 
byte CameraUP[]={0xFE, 0x01, 0x01, (byte)0x00, 0x03};//270 Degrees Lens is on top of bord
byte AngleConfirm[]={0xFE, (byte)0x00, (byte)0x00, (byte)0x00};

//areas
int leftP=-4;
int leftY=4;
int centerP=-8;
int centerY=-4;
int rightP=-8;
int rightY=-5;
int bottomP=-8;
int bottomY=0;

//Actuators LEDs or mussle wires.
int leftLED=2;
int centerLED=5;
int rightLED=3;
int bottomLED=4;

byte omronReadings[30];
//Gaze
int pitch=0;
int yaw=0;

//Gender
int gender=0;
int confidence=0;

//delays
int actuatorTime=5000;
int wait=10000;//amout of time needed for cooldown
long waitL=0;
long waitR=0;
long waitC=0;
long waitB=0;

SoftwareSerial mySerial(rxPin, txPin); // RX, TX

void setup()  {
  // define pin modes for SoftwareSerial tx, rx pins:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial.println("Serial Initialized");

  //Initialize actuators 
  pinMode(leftLED, OUTPUT);
  pinMode(centerLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(bottomLED, OUTPUT);
  
  //flipping camera to proper viewing angle
  mySerial.write(CameraDown, sizeof(CameraDown));
  delay (10);
  mySerial.write(AngleConfirm, sizeof(AngleConfirm));
  delay (10);
  Serial.println("Camera Ready");
}

void loop(){
  while (Serial.available()>0){
    int action=Serial.read();
    switch (action) {
    case 'a':    
      mySerial.write(gazeGen, sizeof(gazeGen));
      break;
    //case 'b':    
      //Serial.write(rightLED, HIGH);
      //break;
    } 
  }
  mySerial.write(gazeGen, sizeof(gazeGen));
  readGazeGender();
  if(pitch>leftP && yaw>leftY && (millis()-waitL)>wait){
    waitL=millis();
    digitalWrite(leftLED, HIGH);
  }

  if(pitch>rightP && yaw<rightY && (millis()-waitR)>wait){
    waitR=millis();
    digitalWrite(rightLED, HIGH);
  }

  if(pitch>rightP && yaw>rightY&&yaw<leftY & yaw!=0 && (millis()-waitC)>wait){
    waitC=millis();
    digitalWrite(centerLED, HIGH);
  }

  if(pitch<bottomP && (millis()-waitB)>wait){
    waitB=millis();
    digitalWrite(bottomLED, HIGH);
  }
  
  powerDownActuators();
  delay(300);// prevent from fetching too much data and loosing comunication
}


//Shuting Actuators off,I am not sure how long the muscle wire have to be on,
//assuiming it is more than just an instant use this function.
void powerDownActuators(){
  if(millis()-waitL>actuatorTime){
    digitalWrite(leftLED,LOW);
  }
  if(millis()-waitR>actuatorTime){
    digitalWrite(rightLED,LOW);
  }
  if(millis()-waitC>actuatorTime){
    digitalWrite(centerLED,LOW);
  }
  if(millis()-waitB>actuatorTime){
    digitalWrite(bottomLED,LOW);
  }
  pitch=0;
  yaw=0;
}


