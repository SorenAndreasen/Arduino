
// GYRO INFO UD AF MMA7361

const float alpha = 0.5; // smoothing

double fXg = 0;
double fYg = 0;
double fZg = 0;

void setup()
{
  Serial.begin(9600);
  digitalWrite(2, LOW); // g sel
  digitalWrite(3, HIGH); // sleep

  delay(100);
}

void loop()
{
  
  double pitch, roll, Xg, Yg, Zg;
  
  Zg = analogRead(/* INDSÆT PIN FOR "Z" AKSE */); 
  Yg = analogRead(/* INDSÆT PIN FOR "Y" AKSE */);
  Xg = analogRead(/* INDSÆT PIN FOR "X" AKSE */);
  
  // low pass filter
  fXg = Xg * alpha + (fXg * (1.0 - alpha));
  fYg = Yg * alpha + (fYg * (1.0 - alpha));
  fZg = Zg * alpha + (fZg * (1.0 - alpha));
  
  // roll and pitch equations
  
  roll = (atan2(-fYg, fZg) * 180.0) / M_PI;
  pitch = (atan2(fXg, sqrt(fYg*fYg + fZg * fZg)) * 180) / M_PI;

 Serial.print("pitch:           ");
 Serial.println(pitch);
  
 Serial.print("roll: ");
 Serial.println(roll);
    delay(10);
  
}
