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


void rain () 
{


}

void display_menu ()
{
  char * out = "test"; 
  for(int i = 0; i < 30; i++)
  { 
  OrbitOledClear();
  OrbitOledMoveTo(i,10);
  OrbitOledDrawString(out);
  OrbitOledPutBmp(5,1,fill);
  OrbitOledUpdate();
  delay(200);
  }  
} 






