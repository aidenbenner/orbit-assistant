#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitBoosterPackDefs.h>
#include <delay.h>
#include <LaunchPad.h>
#include <FillPat.h>
#include <string.h>

void setup ()
{
  Serial.begin (9600);
  init_sensors ();

  OrbitOledInit ();
  OrbitOledClear ();
  OrbitOledClearBuffer ();
  OrbitOledSetFillPattern (OrbitOledGetStdPattern (iptnSolid));
  OrbitOledSetDrawMode (modOledSet);
}

void init_screen ()
{
  OrbitOledInit ();
  OrbitOledClear ();
  OrbitOledClearBuffer ();
  OrbitOledSetFillPattern (OrbitOledGetStdPattern (iptnSolid));
  OrbitOledSetDrawMode (modOledSet);
}

void loop ()
{
  print_sensor_test ();
  delay (100);
} 
