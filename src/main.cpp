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

enum GameState {
  TITLE, INGAME, HIGHSCORE
};

void init_device (){
  WireInit ();
  serial_init ();
  oled_init ();
  accel_init ();
}

void setup ()
{
  init_device ();
}

void loop ()
{
  display_title ();
  accel_tick ();
}

