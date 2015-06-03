
// servo
#include <Servo.h>
Servo servo1;

int incomingByte[3];

// piezo variables
int midiNote = 0;
int pwmPin = 9;
float frequency = 0;
boolean playState = 0;
int freqConversion[128]        = {8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 19, 21, 
                                    22, 23, 24, 26, 28, 29, 31, 33, 35, 37, 39, 41, 44, 46, 49, 52, 
                                    55, 58, 62, 65, 69, 73, 78, 82, 87, 92, 98, 104, 110, 117, 123, 
                                    131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 262, 
                                    277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523, 554, 
                                    587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 
                                    1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 
                                    2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 
                                    4186, 4435, 4699, 4978, 5274, 5588, 5920, 5920, 6645, 7040, 7459, 
                                    7902, 8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544};



void setup()
{
  for(int i=2; i < 53; i++)
  {
    pinMode(i, OUTPUT); // all outputs
    digitalWrite(i, LOW); // set to off
  }
  

  
  Serial.begin(115200);
  servo1.attach(2);
  //servo1.setMaximumPulse(2000);
  //servo1.setMinimumPulse(700);
  
}

void loop()
{
  if(Serial.available() == 3)
  {
    incomingByte[0] = Serial.read();
    incomingByte[1] = Serial.read();
    incomingByte[2] = Serial.read();
    
    if(incomingByte[0] == 12) // servo
    {
      servo1.write(incomingByte[1]);
    }
    else if(incomingByte[0] == 13) // piezo player
    {
     if((incomingByte[2] > 0) && (playState == 0)) { 
     
      midiNote = incomingByte[1];    // set midiNote to current midi pitch
  
      frequency = freqConversion[midiNote]; // Convert to Hz (bob bob.. no decimals..)
  
      tone(pwmPin, frequency);
      
      playState = 1;
     }
     
     else {
       noTone(pwmPin);  // stop oscillator  
       playState = 0;   // if velocity == 0, set 'not playing state'
     }
      
    }
    else // solenoids
    {
      if(incomingByte[1] == 1) digitalWrite(incomingByte[0], HIGH);
      else digitalWrite(incomingByte[0], LOW);
      
      Serial.print("byte0: ");
      Serial.println(incomingByte[0]);
      
      Serial.print("byte1: ");
      Serial.println(incomingByte[1]);
    }
  }
}
