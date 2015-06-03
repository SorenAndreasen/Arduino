
// GYRO INFO UD AF ADXL345
#include <Wire.h>
#include <ADXL345.h> // -- HENT DETTE BIBLIOTEK!

const float alpha = 0.5; // smoothing

double fXg = 0;
double fYg = 0;
double fZg = 0;

ADXL345 acc;

void setup()
{
  acc.begin();
  Serial.begin(9600);
  delay(100);
}

void loop()
{
  
  double pitch, roll, Xg, Yg, Zg;
  
  acc.read(&Xg, &Yg, &Zg);
  
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
