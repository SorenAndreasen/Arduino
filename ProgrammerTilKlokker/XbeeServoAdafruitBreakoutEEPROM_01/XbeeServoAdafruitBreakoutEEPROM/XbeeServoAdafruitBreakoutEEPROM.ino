#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  340 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  390 // this is the 'maximum' pulse length count (out of 4096)

#define BELLMIN 100
#define BELLMAX 600

// time
long previousBell1Hit = 0;
long previousBell2Hit = 0;
long previousBell3Hit = 0;
int interval = 150; // tiden der går fra at bell servoerne trækker sig tilbage, i millisekunder
unsigned long currentMillis = 0;

int Clock0Bell_OffPosition = 90; 
int Clock1Bell_OffPosition = 90;
int Clock2BellA_OffPosition = 90;
int Clock2BellB_OffPosition = 90;

int Clock0Bell_OnPosition = 120;
int Clock1Bell_OnPosition = 120;
int Clock2BellA_OnPosition = 120;
int Clock2BellB_OnPosition = 120;

long previousBellHit[4];
int bellActive[4];
int incomingByte[3];
// 0: which command (0-2: clocks, 3: bell timing, 4: write bell positions to EEPROM)
// 1: which servo (0 = pointers, 1 = pendulum, 2 = bell)
// 2: value

void setup()  // setup, køres 1 gang som det første hver gang arduinoen får strøm
{  
 
  Serial.begin(115200);
  
  if(EEPROM.read(0) != 255) // if EEPROM has been written to, read bell positions
  {
    Serial.println("readingEEPROM");
    Clock0Bell_OffPosition = EEPROM.read(1);
    Clock1Bell_OffPosition = EEPROM.read(2);
    Clock2BellA_OffPosition = EEPROM.read(3);
    Clock2BellA_OffPosition = EEPROM.read(4);
    Clock0Bell_OnPosition = EEPROM.read(5);
    Clock1Bell_OnPosition = EEPROM.read(6);
    Clock2BellA_OnPosition = EEPROM.read(7);
    Clock2BellB_OnPosition = EEPROM.read(8);
    interval = EEPROM.read(9);
    Serial.print("bell1off ");
    Serial.println(Clock0Bell_OffPosition);
    Serial.print("bell2off ");
    Serial.println(Clock1Bell_OffPosition);
    Serial.print("bell3aoff ");
    Serial.println(Clock2BellA_OffPosition);
    Serial.print("bell3boff ");
    Serial.println(Clock2BellB_OffPosition);
    Serial.print("interval ");
    Serial.println(interval);
    Serial.print("bell1on ");
    Serial.println(Clock0Bell_OnPosition);
    Serial.print("bell2on ");
    Serial.println(Clock1Bell_OnPosition);
    Serial.print("bell3aon ");
    Serial.println(Clock2BellA_OnPosition);
    Serial.print("bell3bon ");
    Serial.println(Clock2BellB_OnPosition);
    
  }
  
 pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  /////////// CLOCK 1 \\\\\\\\\\\\
  // Pointers
  uint16_t pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(0, 0, pulselen);
  // Pendulum
  pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(1, 0, pulselen);
  // Bell
  pulselen = map(Clock0Bell_OffPosition, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(2, 0, pulselen);
  
  /////////// CLOCK 2 \\\\\\\\\\\\
  // Pointers
  pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(3, 0, pulselen);
  // Pendulum
  pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(4, 0, pulselen);
  // Bell
  pulselen = map(Clock1Bell_OffPosition, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(5, 0, pulselen);
  
  /////////// CLOCK 3 \\\\\\\\\\\\
  // Pointers
  pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(6, 0, pulselen);
  // Pendulum
  pulselen = map(90, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(7, 0, pulselen);
  // Bell A
  pulselen = map(Clock2BellA_OffPosition, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(8, 0, pulselen);
  // Bell B
  pulselen = map(Clock2BellB_OffPosition, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(9, 0, pulselen);
  
}

void loop() {
  
  currentMillis = millis();
  
  while(Serial.available() > 3) // hvis arduinoen ikke kan følge med printer den "overload" til max
  {
    int discardSerial;
    discardSerial = Serial.read(); // throw away if serial gets overloaded
    Serial.println("overload");
  }
  
  if(Serial.available() == 3 )
  {
     incomingByte[0] = Serial.read();
     incomingByte[1] = Serial.read();
     incomingByte[2] = Serial.read();
   
  // debug:
     Serial.print("0 is: ");
     Serial.println(incomingByte[0]);
     Serial.print("1 is: ");
     Serial.println(incomingByte[1]);
     Serial.print("2 is: ");
     Serial.println(incomingByte[2]);

   if(incomingByte[0] == 0) // clock 0
   {
     if(incomingByte[1] == 0) // pointers servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(0, 0, pulselen);
     }
     else if(incomingByte[1] == 1) // pendulum servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(1, 0, pulselen); 
     } // bell:
     else if(incomingByte[1] == 2) triggerBell(0); // trigger bell
     else if(incomingByte[1] == 3) Clock0Bell_OffPosition = incomingByte[2]; // set new off pos
     else if(incomingByte[1] == 4) Clock0Bell_OnPosition = incomingByte[2]; // set new on pos
   }
   else if(incomingByte[0] == 1) // clock 1
   {
     if(incomingByte[1] == 0) // pointers servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(3, 0, pulselen);
     }
     else if(incomingByte[1] == 1) // pendulum servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(4, 0, pulselen); 
     } // bell:
     else if(incomingByte[1] == 2) triggerBell(1); // trigger bell
     else if(incomingByte[1] == 3) Clock1Bell_OffPosition = incomingByte[2]; // set new off pos
     else if(incomingByte[1] == 4) Clock1Bell_OnPosition = incomingByte[2]; // set new on pos
   }
   else if(incomingByte[0] == 2) // clock 2
   {
     if(incomingByte[1] == 0) // pointers servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(6, 0, pulselen);
     }
     else if(incomingByte[1] == 1) // pendulum servo
     {
       uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(7, 0, pulselen); 
     } // bell:
     else if(incomingByte[1] == 2) triggerBell(2); // trigger bell A
     else if(incomingByte[1] == 3) triggerBell(3); // trigger bell B
     else if(incomingByte[1] == 4) Clock2BellA_OffPosition = incomingByte[2]; // set new off pos bell A
     else if(incomingByte[1] == 5) Clock2BellB_OffPosition = incomingByte[2]; // set new off pos bell B
     else if(incomingByte[1] == 6) Clock2BellA_OnPosition = incomingByte[2]; // set new on pos bell A
     else if(incomingByte[1] == 7) Clock2BellB_OnPosition = incomingByte[2]; // set new on pos bell A
   }
   else if(incomingByte[0] == 3) // change bell timing
   {
     interval = incomingByte[2] * 50;
     Serial.print("new bell speed: ");
     Serial.println(interval);
   }
   else if(incomingByte[0] == 4) // save bell positions in EEPROM
   {
      Serial.println("Writing EEPROM");
      EEPROM.write(1, Clock0Bell_OffPosition);
      EEPROM.write(2, Clock1Bell_OffPosition);
      EEPROM.write(3, Clock2BellA_OffPosition);
      EEPROM.write(4, Clock2BellB_OffPosition);
      
      EEPROM.write(5, Clock0Bell_OnPosition);
      EEPROM.write(6, Clock1Bell_OnPosition);
      EEPROM.write(7, Clock2BellA_OnPosition);
      EEPROM.write(8, Clock2BellB_OnPosition);
      EEPROM.write(9, interval);
    
      EEPROM.write(0, 1); // set has-been-written-to flag
   }
   else if(incomingByte[0] == 5)
   {
      uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(0, 0, pulselen);
  
    uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(3, 0, pulselen);
  
     uint16_t pulselen = map(incomingByte[2], 0, 180, SERVOMIN, SERVOMAX);
       pwm.setPWM(6, 0, pulselen);
   
   }
   
  }
  
   
  if(bellActive[0] == 1 && currentMillis - previousBellHit[0] > interval)
  {
    //Serial.print("bell0Reset to ");
    //Serial.println(Clock0Bell_OffPosition);
//    previousBellHit[0] = currentMillis;
    bellActive[0] = 0;
    uint16_t pulselen = map(Clock0Bell_OffPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(2, 0, pulselen);  
  }
  if(bellActive[1] == 1 && currentMillis - previousBellHit[1] > interval)
  {
    //Serial.print("bell1Reset to ");
    //Serial.println(Clock1Bell_OffPosition);
    previousBellHit[1] = currentMillis;
    bellActive[1] = 0;
    uint16_t pulselen = map(Clock1Bell_OffPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(5, 0, pulselen);
  }
  if(bellActive[2] == 1 && currentMillis - previousBellHit[2] > interval)
  {
    //Serial.print("bell2AReset to ");
    //Serial.println(Clock2BellA_OffPosition);
    previousBellHit[2] = currentMillis;
    bellActive[2] = 0;
    uint16_t pulselen = map(Clock2BellA_OffPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(8, 0, pulselen);
  }
  if(bellActive[3] == 1 && currentMillis - previousBellHit[3] > interval)
  {
    //Serial.print("bell2BReset to ");
    //Serial.println(Clock2BellB_OffPosition);
    previousBellHit[2] = currentMillis;
    bellActive[3] = 0;
    uint16_t pulselen = map(Clock2BellB_OffPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(9, 0, pulselen);
  }
}

void triggerBell(int bell) 
{
  if(bell == 0) // clock 0
  {
    //Serial.println("bell0Triggered");
    previousBellHit[0] = currentMillis;
    uint16_t pulselen = map(Clock0Bell_OnPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(2, 0, pulselen); 
    bellActive[0] = 1;
  }
  else if(bell == 1) // clock 1
  {
    //Serial.println("bell1Triggered");
    previousBellHit[1] = currentMillis;
    uint16_t pulselen = map(Clock1Bell_OnPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(5, 0, pulselen); 
    bellActive[1] = 1;
  }
  else if(bell == 2) // clock 2 A
  {
    //Serial.println("bell2Triggered");
    previousBellHit[2] = currentMillis;
    uint16_t pulselen = map(Clock2BellA_OnPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(8, 0, pulselen); 
    bellActive[2] = 1;
  }
  else if(bell == 3) // clock 2 B
  {
    //Serial.println("bell3Triggered");
    previousBellHit[3] = currentMillis;
    uint16_t pulselen = map(Clock2BellB_OnPosition, 0, 180, BELLMIN, BELLMAX);
    pwm.setPWM(9, 0, pulselen); 
    bellActive[3] = 1;
  }
}


