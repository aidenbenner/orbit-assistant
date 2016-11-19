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
#include "sensors.h"

char fill[] = {
  0xFF,  
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF 
};

char sun [] = {
  0xFF,  
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF 
}

char * test_string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

void display_long_string(char * input) {
  //need to split string into line sections

  OrbitOledDrawString(out);
}

void display_menu ()
{
  char * out = "test"; 
  for(int i = 0; i < 30; i++)
  { 
    OrbitOledClear ();
    OrbitOledMoveTo (i,10);
    OrbitOledDrawString (out);
    OrbitOledPutBmp (5,1,fill);
    OrbitOledUpdate ();
    delay (200);
  }  
} 






