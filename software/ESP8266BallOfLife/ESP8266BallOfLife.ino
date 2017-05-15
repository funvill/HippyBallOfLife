//
// Demo mode will slowly cycle thought several patterns. When the balls is moved.
// The accelerometer controlls the RGB of the lights.
//
// Two modes
// 1) Demo mode
// 2) accelerometer mode

int mode;
const int MODE_ACCELEROMETER = 0;

// MPU-6050 Short Example Sketch
#include <Wire.h>
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Check to see if the thing has moved in the last x seconds
const unsigned char SETTING_MOVE_TIMER = 3;
const unsigned char SETTING_MOVE_AMOUNT = 10;
unsigned long moving_last_time;
uint8_t moving_last_a, moving_last_b, moving_last_c;

// LED
#include "FastLED.h"

// Game Settings
// ----------------------------------------------------------------------------
// The number of LEDs in this strip.
static const unsigned char SETTINGS_NUM_LEDS = 30;
static const int SETTINGS_MIN_ACCEL = -30000;
static const int SETTINGS_MAX_ACCEL = +30000;

// Pins
static const unsigned char SETTING_PIN_LED_DATA = D4;

// Debug
static const unsigned short SETTING_SERIAL_BAUD_RATE = 9600;

// The current status of all the LEDS
CRGB leds[SETTINGS_NUM_LEDS];

// Setup
void setup()
{
    Serial.begin(SETTING_SERIAL_BAUD_RATE);
    FastLED.addLeds<NEOPIXEL, SETTING_PIN_LED_DATA>(leds, SETTINGS_NUM_LEDS);

    // Set up accelerometer
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void Reset()
{
    // Init settings.
    mode = MODE_ACCELEROMETER;
    moving_last_time = 0;
    moving_last_a = 0;
    moving_last_b = 0;
    moving_last_c = 0;

    // Set all the LEDS to black
    for (unsigned short ledOffset = 0; ledOffset < SETTINGS_NUM_LEDS; ledOffset++) {
        leds[ledOffset] = CRGB::Black;
    }

    Serial.println("Reset()");
}

// ToDo: this is not working yet 
bool GetMoving() {
  if( millis() - moving_last_time < SETTING_MOVE_TIMER * 1000 ) {
    return true ;
  }
  return false; 
}

void UpdateInput()
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void DebugPrint()
{
    
    Serial.print(" | AcX = ");
    Serial.print(AcX);
    Serial.print(" | AcY = ");
    Serial.print(AcY);
    Serial.print(" | AcZ = ");
    Serial.print(AcZ);
    /*
    Serial.print(" | Tmp = ");
    Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet    
    Serial.print(" | GyX = ");
    Serial.print(GyX);
    Serial.print(" | GyY = ");
    Serial.print(GyY);
    Serial.print(" | GyZ = ");
    Serial.print(GyZ);
    */
}

void ModeAccelerometer()
{
    // Scale the values to a 0-254 value 
    uint8_t a = (uint8_t)map(AcX, SETTINGS_MIN_ACCEL, SETTINGS_MAX_ACCEL, 0, 254 );
    uint8_t b = (uint8_t)map(AcY, SETTINGS_MIN_ACCEL, SETTINGS_MAX_ACCEL, 0, 254 );
    uint8_t c = (uint8_t)map(AcZ, SETTINGS_MIN_ACCEL, SETTINGS_MAX_ACCEL, 0, 254 );

    // Debug print. 
    Serial.print(" | a = ");
    Serial.print(a);
    Serial.print(" | b = ");
    Serial.print(b);
    Serial.print(" | c = ");
    Serial.print(c);

    // Check for movement
    // If we have sufiiant movment in this frame then update the last movment timer
    // This timer is used to tell if we have moved the ball in the last three seconds or so. 
    if (abs(moving_last_a - a) > SETTING_MOVE_AMOUNT ) {
        moving_last_time = millis();
    }
    if (abs(moving_last_b - b) > SETTING_MOVE_AMOUNT ) {
        moving_last_time = millis();
    }
    if (abs(moving_last_b - b) > SETTING_MOVE_AMOUNT ) {
        moving_last_time = millis();
    }
    moving_last_a = a ; 
    moving_last_b = b ; 
    moving_last_c = c ; 

    // We have three numbers that need to be used for the hue. Lets take all three
    // numbers add them together and divide by 3. There will be multiple ways to get 
    // to get to the same color but thats okay. 
    for (unsigned short ledOffset = 0; ledOffset < SETTINGS_NUM_LEDS; ledOffset++) {
        leds[ledOffset] = CHSV(a/3 + b/3 + c/3, 255, 255);
    }
    FastLED.show();
}

void loop()
{
    Serial.print( millis() );

    UpdateInput();
    DebugPrint();

    ModeAccelerometer();

    // Debug print 
    Serial.print(" | Moving = ");
    Serial.print(GetMoving());
    Serial.println("");
}
