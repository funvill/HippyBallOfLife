#include <Adafruit_NeoPixel.h>
#include "Adafruit_CircuitPlayground.h"

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
  
  Serial.begin(115200);
  Serial.println("Ball of life");

  // Initialize Circuit Playground library.
  CircuitPlayground.begin();  

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}





// Define range of possible acceleration values.
#define MIN_ACCEL -10.0
#define MAX_ACCEL 10.0

// Define range of colors (min color and max color) using their red, green, blue components.
// First the min color:
#define MIN_COLOR_RED   0x00
#define MIN_COLOR_GREEN 0x00
#define MIN_COLOR_BLUE  0x00

// Then the max color:
#define MAX_COLOR_RED   0xFF
#define MAX_COLOR_GREEN 0xFF
#define MAX_COLOR_BLUE  0xFF

// Define each mode with the following interface for a loop and modePress
// function that will be called during the main loop and if the mode button
// was pressed respectively.  It's up to each mode implementation to fill
// these in and add their logic.
class Demo {
public:
  virtual ~Demo() {}

  virtual void loop() = 0;
  virtual void modePress() = 0;
};

// Linear interpolation function is handy for all the modes to use.
float lerp(float x, float xmin, float xmax, float ymin, float ymax) {
  if (x >= xmax) {
    return ymax;
  }
  if (x <= xmin) {
    return ymin;
  }
  return ymin + (ymax-ymin)*((x-xmin)/(xmax-xmin));
}


class TiltDemo: public Demo {
public:
  TiltDemo() {mode=0;}
  ~TiltDemo() {}

  virtual void loop() {
    // Grab the acceleration for the current mode's axis.
    float accel = 0;
    float acce2 = 0;
    float acce3 = 0;

    accel = CircuitPlayground.motionX();
    acce2 = CircuitPlayground.motionY();
    acce3 = CircuitPlayground.motionZ();

    
    switch (mode) {
      case 0:
        break;
      case 1:
        break;
      case 2:
        
        break;
    }

    // Now interpolate the acceleration into a color for the pixels.
    uint8_t red = (uint8_t)lerp(accel, MIN_ACCEL, MAX_ACCEL, MIN_COLOR_RED, MAX_COLOR_RED);
    uint8_t green = (uint8_t)lerp(acce2, MIN_ACCEL, MAX_ACCEL, MIN_COLOR_GREEN, MAX_COLOR_GREEN);
    uint8_t blue = (uint8_t)lerp(acce3, MIN_ACCEL, MAX_ACCEL, MIN_COLOR_BLUE, MAX_COLOR_BLUE);

    Serial.print("red: "); Serial.print(red, DEC); 
    Serial.print(" green: "); Serial.print(green, DEC); 
    Serial.print(" blue: "); Serial.print(blue, DEC); 
    Serial.println("");
    

    // Light up all the pixels the interpolated color.
    for (int i=0; i<strip.numPixels(); ++i) {
      strip.setPixelColor(i, red, green, blue);
      
    }
    strip.show();
  }

  virtual void modePress() {
    // Change the mode (axis being displayed) to a value inside 0-2 for X, Y, Z.
    mode += 1;
    if (mode > 2) {
      mode = 0;
    }
  }

private:
  int mode = 0;
};


void MakeRed() {
  for (int i=0; i<strip.numPixels(); ++i) {      
      strip.setPixelColor(i, 0xFF, 0, 0);      
    }
    strip.show();
}

TiltDemo d; 
void loop() {
  // MakeRed();
  d.loop();
}


