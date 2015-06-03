#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 
  
 sweep();

}


void sweep(){
  
  for(int i = 0; i < 50; i++)
  {
    int f2 = 255; // previous led
    for(int f = 0; f < 255; f = f+4) // fade
    {
      strip.setPixelColor(i, f,f,f); // fade up
      
      if(i>0) 
      {
        strip.setPixelColor(i-1, f2,f2,f2); // fade down
        f2 = f2-4;
        if(f2 < 4) strip.setPixelColor(i-1, 0,0,0); // fade down
      }
     
      delayMicroseconds(1);
      strip.show();
    }
  }
  
  for(int f = 255; f > 0; f--)
  {
    strip.setPixelColor(49, f,f,f); // fade down
    delayMicroseconds(1);
    if(f < 4) strip.setPixelColor(49, 0,0,0); 
    strip.show();
  }
  
}


