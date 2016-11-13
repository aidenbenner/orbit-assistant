#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "oled.h"
#include "serial.h"
#include "sensors/accel.h"
#include "sensors/Wire_Util.h"

void setup()
{
  WireInit();
  Serial.begin(9600);
  accel_init ();
}

void loop()
{
  accel_tick ();
}

