#include <Servo.h>
#include <stdint.h>
#include "TouchScreen.h"
#include <PID_v1.h>

#define BOUND 25

#define YP A2     // must be an analog pin, use "An" notation!
#define XM A3     // must be an analog pin, use "An" notation!
#define YM 8      // can be a digital pin
#define XP 9      // can be a digital pin
#define XServoPin 7  //
#define YServoPin 6  //

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 10);

Servo xServo;
Servo yServo;
unsigned long time;
unsigned long stable = 0;
unsigned int noTouchCount = 0;

double SetpointX = 200, InputX = 200 , OutputX = 80;
//double xKp = 0.38  , xKd = 0.152 , xKi = 4*xKd;
double xKp = 0.6  , xKd = 0.178 , xKi = 4*xKd;  

double SetpointY = 150, InputY = 150 , OutputY = 90;
//double yKp =0.29, yKd = 0.055 , yKi = 4*yKd;
double yKp =0.42, yKd = 0.065 , yKi = 4*yKd;

PID xPID(&InputX, &OutputX, &SetpointX, xKp, xKi, xKd, DIRECT);
PID yPID(&InputY, &OutputY, &SetpointY, yKp, yKi, yKd, DIRECT);

void setup(void) {

  xServo.attach(XServoPin);
  xServo.write(80);
  xPID.SetMode(AUTOMATIC);
  xPID.SetSampleTime(40);
   
  yServo.attach(YServoPin);
  yServo.write(90);
  yPID.SetMode(AUTOMATIC);
  yPID.SetSampleTime(30);
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

      InputX = map(p.x, 125, 965, 0, 400);
      InputY = map(p.y, 130, 910, 300, 0);//  
             
      noTouchCount = 0;
      if ( (InputX > SetpointX - BOUND && InputX < SetpointX + BOUND  && InputY > SetpointY - BOUND && InputY < SetpointY + BOUND) )
      {
        stable++;
      }
      
      xPID.Compute();
      yPID.Compute();
      OutputY = map(OutputY,0,255,60,120);
      OutputX = map(OutputX,0,255,55,125);
     /* Serial.print("   InputX:"); Serial.print(InputX);
      Serial.print("   InputY:"); Serial.print(InputY);
      Serial.print("   Xoutput1:");Serial.print(OutputX);
      Serial.print("   Youtput1:");Serial.println(OutputY);*/
    } else // Ball is not on plate
    {
      //Serial.print("No touch:"); Serial.println(noTouchCount);
      noTouchCount++;
      if (noTouchCount == 75)
      {
        noTouchCount++;
        OutputX = 80;
        OutputY = 90;
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

    xServo.write(OutputX); // outputX
    yServo.write(OutputY);
  }

  xServo.detach();
  yServo.detach();

  while (stable == 125)
  {
    //Serial.print("Stable");
    TSPoint p = ts.getPoint();
    
    InputX = map(p.x, 125, 965, 0, 400);
    InputY = map(p.y, 130, 910, 300, 0);

    if ( ( InputX > SetpointX -BOUND && InputX < SetpointX + BOUND  && InputY > SetpointY - BOUND && InputY < SetpointY + BOUND) )
    {
      xServo.attach(XServoPin);
      yServo.attach(YServoPin);
    }else stable = 0;
  }
}


