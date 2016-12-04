#include <Servo.h>
#include <stdint.h>
#include "TouchScreen.h"
#include <PID_v1.h>

#define YP A2     // must be an analog pin, use "An" notation!
#define XM A3     // must be an analog pin, use "An" notation!
#define YM 8      // can be a digital pin
#define XP 9      // can be a digital pin
#define XServoPin 7  //
#define YServoPin 6  //

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 10);

Servo xServo;
Servo yServo;

unsigned long stable = 0;
unsigned int noTouchCount = 0;

double SetpointX = 90, InputX = 80 , OutputX = 80;
double xKp = 0.6 , xKi = 0.06 , xKd = 0.26;
int statuss = 0;
int oldStatus = 0;

double SetpointY = 90, InputY = 80 , OutputY = 80;
double yKp = 1, yKi = 0.08 , yKd = 0.13;

PID xPID(&InputX, &OutputX, &SetpointX, xKp, xKi, xKd, DIRECT);
PID yPID(&InputY, &OutputY, &SetpointY, yKp, yKi, yKd, DIRECT);

void setup(void) {

  digitalWrite(8, LOW); //LED INIT
  digitalWrite(9, LOW);

  xServo.attach(XServoPin);
  xServo.write(80);
  xPID.SetMode(AUTOMATIC);
  xPID.SetOutputLimits(40, 140);
  //xPID.SetSampleTime(50);
  yServo.attach(YServoPin);
  yServo.write(80);
  yPID.SetMode(AUTOMATIC);
  yPID.SetOutputLimits(40, 140);
  //yPID.SetSampleTime(50);
  delay(2000);
  Serial.begin(9600);
}

void loop(void) {

  while (stable < 125)
  {

    TSPoint p = ts.getPoint();
    if ( p.x != 0 && p.y != 1023 ) // Ball is on plate
    {
      xServo.attach(XServoPin);
      yServo.attach(YServoPin);

      InputX = map(p.x, 125, 964, 40, 140);
      InputY = map(p.y, 130, 920, 140, 40);
      int gapX = abs(SetpointX - InputX);
      Serial.print("Gap : ");Serial.println(gapX);
      if(gapX < 20)
          xPID.SetTunings(xKp / 2 , xKi / 2,xKi / 2);
      else
          xPID.SetTunings(xKp * 2 , xKi * 2,xKi * 2);
          
      noTouchCount = 0;
      if ( ( InputX > SetpointX - 2 && InputX < SetpointX + 2  && InputY > SetpointY - 2 && InputY < SetpointY + 2) )
      {
        stable++;
      }
   
      xPID.Compute();
      yPID.Compute();

//      Serial.print("   P.X:"); Serial.print(p.x);
//      Serial.print("   Input.:"); Serial.print(InputX);
//      Serial.print("   X output:");Serial.print(OutputX);
//      Serial.print("   P.Y:"); Serial.print(p.y);
//      Serial.print("   Input.:"); Serial.print(InputY);
//      Serial.print("   Y output:");Serial.println(OutputY);
  
    } else // Ball is not on plate
    {
      Serial.print("No touch:"); Serial.println(noTouchCount);
      noTouchCount++;
      if (noTouchCount == 50)
      {
        noTouchCount++;
        OutputX = 80;
        OutputY = 80;
        xServo.write(OutputX);
        yServo.write(OutputY);
        delay(100);
      }
      if (noTouchCount == 150) //if there is no ball on plate longer
      {
        xServo.detach(); //detach servos
        yServo.detach();
      }
    }

    xServo.write(OutputX);
    yServo.write(80);
  }

  xServo.detach();
  yServo.detach();

  while (stable == 125)
  {
    TSPoint p = ts.getPoint();
   InputX = map(p.x, 128, 964, 40, 140);
      InputY = map(p.x, 130, 920, 140, 40);

    if ( ( InputX > SetpointX - 2 && InputX < SetpointX + 2 && InputY > SetpointY - 2 && InputY < SetpointY + 2) )
    {
      xServo.attach(XServoPin);
      yServo.attach(YServoPin);
      stable = 0;
    }

  }



  //  Serial.print("Pressure : ") ; Serial.println(p.z);
  //  //if (p.z > ts.pressureThreshhold) { // Check pressure.
  //
  //  // Do for X
  //  InputX = map(p.x, 128, 964, 40, 140);
  //  Serial.print("P.X:"); Serial.print(p.x);
  //  Serial.print("  X input:");Serial.print(InputX);
  //  xPID.Compute();
  //  Serial.print("          X output : ") ; Serial.println(OutputX);
  //
  //  // Do for Y
  //  InputY = map(p.y, 139, 914, 40, 140);
  //  yPID.Compute();
  //
  //
  //
  //  yServo.write(OutputY);
  //  xServo.write(OutputX);
  //  Serial.print("P.Y:"); Serial.print(p.y);
  //  Serial.print("  Y input:");Serial.print(InputY);
  //  Serial.print("Pid output for y : ");Serial.println(OutputY);
  //
  //  xServo.detach();
  //  yServo.detach();
  //  delay(100);
}


