
// BIBLLIOTEKER
#include <SPI.h>
#include <Adafruit_PCD8544.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


int xPos = 5;
int yPos = 5;

void setup() 
{
  
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  display.begin();
  display.setContrast(50);
  display.display();
  display.clearDisplay();
  
  // display.clearDisplay();   // clears the screen and buffer
  
  Serial.begin(9600);

}

void loop() 

{
  
  int xInput;
  int yInput;
  int butInput;
  
  xInput = analogRead(A1);
  yInput = analogRead(A2); 
  butInput = digitalRead(A0);
  
  /// HORISONTALT:
  if(xInput > 600) xPos++;
  else if(xInput < 400) xPos--;
  
  /// VERTIKALT:
  if(yInput > 600) yPos++;
  else if(yInput < 400) yPos--;
  
  
  // Sørg for at holde sig indenfor skærmens grænser:
  if(xPos > 10) xPos = 10;
  else if(xPos < 0) xPos = 0;
  if(yPos > 10) yPos = 10;
  else if(yPos < 0) yPos = 0;
  
  
  
  delay(80);
  
  


        display.drawPixel(xPos, yPos, BLACK);
    display.display();
    
    Serial.println(xPos);
    Serial.println(yPos);

  
  

}






