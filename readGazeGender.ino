//bool values=false;

void readGazeGender(){
  while (mySerial.available() > 0) {
  if(Serial.available()<genderBytes) {} // Wait 'till there are 9 Bytes waiting
    for(int n=0; n<genderBytes; n++){
    omronReadings[n] = mySerial.read(); 
    }
  }
  if (omronReadings[genderBytes-2]!=255 &&  omronReadings[genderBytes-1]!=255){
    pitch=omronReadings[genderBytes-1];
    if (pitch>128){
      pitch=pitch-256;
    }
    yaw=omronReadings[genderBytes-2];
    if (yaw>128){
      yaw=yaw-256;
    }
    confidence=(omronReadings[genderBytes-3]*256)+omronReadings[genderBytes-4];
    gender=omronReadings[genderBytes-5];
    
    for (int i=0; i<genderBytes;i++){
      omronReadings[i]=0;
     }
    
    Serial.print("Yaw: ");  Serial.println(yaw);
    Serial.print("Pitch:");  Serial.println(pitch);
    Serial.print("Gender: "); Serial.println(gender);
    Serial.print("Confidence: ");Serial.print(confidence);
    Serial.println();
  }
  return;
}  


