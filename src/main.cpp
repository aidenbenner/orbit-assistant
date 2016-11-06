#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "oled.h"
#include "serial.h"

void setup()
{
  init_serial ();
  // initialize LED digital pin as an output.
  pinMode (BLUE_LED, OUTPUT);
  init_oled ();
}

void loop()
{
  serial_test ();
  draw_random_shit ();
  OrbitOledUpdate ();
}
