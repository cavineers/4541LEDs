#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>  
#endif
#include "stdlib.h"
#define PIN 5

#define NUM_LEDS 44 

#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(0, 0, 255), 50); // Blue

 blueOverRainbow(20,75,5);  //need black or no background, "tail program"

 pulseBlue(5); 

 fullBlue();
 //delay(2000);

 rainbowFade2Blue(3,3,1); //good

   Snake_v4(0,0,255,50); // same as blueOverRainbow; except it divergents at the 4
   //total time is 37 seconds
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pulseBlue(uint8_t wait) {
  for(int j = 0; j < 256; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j]) );
        }
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j]) );
        }
        delay(wait);
        strip.show();
      }
}


void rainbowFade2Blue(uint8_t wait, int rainbowLoops, int redLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }
        
        strip.show();
        delay(wait);
    }
  
  }



  delay(500);


  for(int k = 0 ; k < redLoops ; k ++){

    for(int j = 0; j < 256 ; j++){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j] ) );
          }
          strip.show();
        }

        delay(2000);
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,neopix_gamma[j] ) );
          }
          strip.show();
        }
  }

  delay(500);


}

void blueOverRainbow(uint8_t wait, uint8_t redSpeed, uint8_t redLength ) {
  
  if(redLength >= strip.numPixels()) redLength = strip.numPixels() - 1;

  int head = redLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor((43-i), strip.Color(0,0,255) );
        }
        else{
          strip.setPixelColor((43-i), strip.Color(0,0,0));
        }
        
      }

      if(millis() - lastTime > redSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
  
}
void fullBlue() {
  
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,255) );
    }
      strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}



void Snake_v4(uint32_t redVal, uint32_t greenVal, uint32_t blueVal, uint8_t wait) {
   for(uint16_t i=0; i<19; ) {
    if (i<19)     strip.setPixelColor(i,   strip.Color(redVal,greenVal,blueVal,0) );
    if ((i+1)<19) strip.setPixelColor(i+1, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i+2)<19) strip.setPixelColor(i+2, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i+3)<19) strip.setPixelColor(i+3, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i+4)<19) strip.setPixelColor(i+4, strip.Color(redVal,greenVal,blueVal,0) );
    strip.show();
    delay(wait);
    if (i<19)     strip.setPixelColor(i,   strip.Color(0,0,0,0) );
    if ((i+1)<19) strip.setPixelColor(i+1, strip.Color(0,0,0,0) );
    if ((i+2)<19) strip.setPixelColor(i+2, strip.Color(0,0,0,0) );
    if ((i+3)<19) strip.setPixelColor(i+3, strip.Color(0,0,0,0) );
    if ((i+4)<19) strip.setPixelColor(i+4, strip.Color(0,0,0,0) );
    strip.show();
    i = i+1;
  }
 
   for(uint16_t i=0,j=0,k=0; i<6; i++) {
    if ((33+j)<strip.numPixels())   strip.setPixelColor(33+j,   strip.Color(redVal,greenVal,blueVal,0) );
    if ((33+j+1)<strip.numPixels()) strip.setPixelColor(33+j+1, strip.Color(redVal,greenVal,blueVal,0) );
    if ((34-k)>18)                  strip.setPixelColor(34-k,   strip.Color(redVal,greenVal,blueVal,0) );
    if ((34-k-1)>18)                strip.setPixelColor(34-k-1, strip.Color(redVal,greenVal,blueVal,0) );
    if ((34-k-2)>18)                strip.setPixelColor(34-k-2, strip.Color(redVal,greenVal,blueVal,0) );
    strip.show();
    delay(wait+150);
    if ((33+j)<strip.numPixels())   strip.setPixelColor(33+j,   strip.Color(0,0,0,0) );
    if ((33+j+1)<strip.numPixels()) strip.setPixelColor(33+j+1, strip.Color(0,0,0,0) );
    if ((34-k)>18)                  strip.setPixelColor(34-k,   strip.Color(0,0,0,0) );
    if ((34-k-1)>18)                strip.setPixelColor(34-k-1, strip.Color(0,0,0,0) );
    if ((34-k-2)>18)                strip.setPixelColor(34-k-2, strip.Color(0,0,0,0) );
    strip.show();
    j = j+2;
    k = k+3;
  }
   for(uint16_t i=5,j=44,k=19; i>0; i--) {
    if (j>(strip.numPixels()-33))   strip.setPixelColor(j,   strip.Color(redVal,greenVal,blueVal,0) );
    if (j>(strip.numPixels()-33+1)) strip.setPixelColor(j-1, strip.Color(redVal,greenVal,blueVal,0) );
    if (j == 36)                   strip.setPixelColor(34, strip.Color(redVal, greenVal, blueVal, 0) );
    if (k<(34+18))                  strip.setPixelColor(k,   strip.Color(redVal,greenVal,blueVal,0) );
    if (k<(34+18+1))                 strip.setPixelColor(k+1, strip.Color(redVal,greenVal,blueVal,0) );
    if (k<(34+18+2))                 strip.setPixelColor(k+2, strip.Color(redVal,greenVal,blueVal,0) );
    strip.show();
    delay(wait+150);
    if (j>(strip.numPixels()-33))   strip.setPixelColor(j,   strip.Color(0,0,0,0) );
    if (j>(strip.numPixels()-33+1)) strip.setPixelColor(j-1, strip.Color(0,0,0,0) );
    if ( j == 36)                   strip.setPixelColor(34, strip.Color(0, 0, 0, 0) );
    if (k<(34+18))                  strip.setPixelColor(k,   strip.Color(0,0,0,0) );
    if (k<(34+18+1))                 strip.setPixelColor(k+1, strip.Color(0,0,0,0) );
    if (k<(34+18+2))                 strip.setPixelColor(k+2, strip.Color(0,0,0,0) );
    strip.show();
    j = j-2;
    k = k+3;
  }
   for(uint16_t i=18; i>0; ) {
    if (i>=0)     strip.setPixelColor(i,   strip.Color(redVal,greenVal,blueVal,0) );
    if ((i-1)>=0) strip.setPixelColor(i-1, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i-2)>=0) strip.setPixelColor(i-2, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i-3)>=0) strip.setPixelColor(i-3, strip.Color(redVal,greenVal,blueVal,0) );
    if ((i-4)>=0) strip.setPixelColor(i-4, strip.Color(redVal,greenVal,blueVal,0) );
    strip.show();
    delay(wait);
    if (i>=0)     strip.setPixelColor(i,   strip.Color(0,0,0,0) );
    if ((i-1)>=0) strip.setPixelColor(i+1, strip.Color(0,0,0,0) );
    if ((i-2)>=0) strip.setPixelColor(i+2, strip.Color(0,0,0,0) );
    if ((i-3)>=0) strip.setPixelColor(i+3, strip.Color(0,0,0,0) );
    if ((i-4)>=0) strip.setPixelColor(i+4, strip.Color(0,0,0,0) );
    strip.show();
    i = i-1;
  }
    
}
 
