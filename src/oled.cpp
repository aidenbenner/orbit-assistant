#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include <Energia.h>
#include "oled.h"

void oled_init ()
{
  OrbitOledInit ();
  OrbitOledClear ();
  OrbitOledClearBuffer ();
  OrbitOledSetFillPattern (OrbitOledGetStdPattern (iptnSolid));
  OrbitOledSetDrawMode (modOledSet);
}

void draw_test (void)
{
  OrbitOledClear ();
  OrbitOledSetCursor (0, 3);
  char ground[] = {
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF
  };
  OrbitOledPutBmp (10, 1, ground);
}


void display_title (void){
  Serial.write("hit");
  char ground[] = {
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF
  };
  OrbitOledClear ();
  OrbitOledSetCursor (0, 3);
  OrbitOledPutBmp (10, 1, ground);
}

