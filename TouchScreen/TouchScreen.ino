// Touch screen library with X Y and Z (pressure) readings as well
// as oversampling to avoid 'bouncing'
// This demo code returns raw readings, public domain

#include <stdint.h>
#include "TouchScreen.h"

#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 10);

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  //if (p.z > ts.pressureThreshhold) {
 // if(p.x != 0 && p.y != 968){
      int x = map(p.x,128,964,0,100);
      int y = map(p.y,139,914,0,100);
     Serial.print("X = "); Serial.print(x);
    Serial.print("\tRealX = "); Serial.print(p.x);

     Serial.print("\tY = "); Serial.print(y);
     Serial.print("\tRealY = "); Serial.print(p.y);
     Serial.print("\tPressure = "); Serial.println(p.z);
  //}
  //}

  delay(1000);
}
