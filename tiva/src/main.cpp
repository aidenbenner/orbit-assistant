#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "oled.h"
#include "serial.h"
#include "accel.h"
#include "Wire_Util.h"
#include "sensors.h"
#include "menu.h"

#define print(x) Serial.println("x")

void device_init (){
  serial_init ();
  WireInit ();
  oled_init ();
  accel_init ();
  init_sensors (); 
  set_all_led(255);
}

void setup ()
{
  device_init ();
}

void loop ()
{
  display_long_string (); 
  delay(50);
}

