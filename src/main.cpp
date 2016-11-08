#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "oled.h"
#include "serial.h"
#include "sensors/accel.h"

void setup()
{
  init_serial ();
  Serial.println("running");
  // initialize LED digital pin as an output.
  pinMode (BLUE_LED, OUTPUT);
  init_oled ();
   accel_init ();
}

void loop()
{
  Serial.println("loop");
  // accel_tick ();
  serial_test ();
  draw_random_shit ();
  OrbitOledUpdate ();
}
