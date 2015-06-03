#include <Servo.h>

// time
long previousBell1Hit = 0;
long previousBell2Hit = 0;
long previousBell3Hit = 0;
long interval = 1502;

unsigned long currentMillis = 0;

long previousBellHit[3];
int bellActive[3];

// clock 1
Servo servo1;
Servo servo2;
Servo servo3;

// clock 2
Servo servo4;
Servo servo5;
Servo servo6;

// clock 3
Servo servo7;
Servo servo8;
Servo servo9;

int incomingByte[3];
// 0: which clock
// 1: which servo (0 = pointers, 1 = pendulum, 2 = bell)
// 2: value

void setup() 
{
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  Serial.begin(9600);
  
  servo1.write(94);
  servo2.write(94);
  servo3.write(1);
  
}
void loop()
{
  currentMillis = millis();
  
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
       servo1.write(incomingByte[2]);
     }
     else if(incomingByte[1] == 1) // pendulum servo
     {
       servo2.write(incomingByte[2]);
     }
     else if(incomingByte[1] == 2) // bell servo
     {
       triggerBell(0, incomingByte[2]);
     }
   }
   else if(incomingByte[0] == 1) // clock 1
   {
     // repeat the above
   }
   else if(incomingByte[0] == 2) // clock 2
   {
     // repeat the above
   }
  
  }

  
  
  if(bellActive[0] == 1 && currentMillis - previousBellHit[0] > interval)
  {
    Serial.println("servo3_Reset");
//    previousBellHit[0] = currentMillis;
    bellActive[0] = 0;
    servo3.write(1);
  }
  if(currentMillis - previousBellHit[1] > interval)
  {
    previousBellHit[1] = currentMillis;
    bellActive[1] = 0;
    servo6.write(1);
  }
  if(currentMillis - previousBellHit[2] > interval)
  {
    previousBellHit[2] = currentMillis;
    bellActive[2] = 0;
    servo9.write(1);
  }
  
  
}


void triggerBell(int clock, int vel) 
{
  
  if(clock == 0) 
  {
    previousBellHit[0] = currentMillis;
 
    servo3.write(30);
    bellActive[0] = 1;
  }
  else if(clock == 1) 
  {
    servo6.write(30);
    bellActive[1] = 1;
  }
  else if(clock == 2) 
  {
    servo9.write(30);
    bellActive[2] = 1;
  }
}
