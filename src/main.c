#include <Energia.h>
#include <OrbitOled.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "oled.h"

void setup()
{
  // initialize LED digital pin as an output.
  pinMode (RED_LED, OUTPUT);
  init_oled ();
}

void loop()
{
  draw_random_shit ();
  OrbitOledUpdate ();
}
