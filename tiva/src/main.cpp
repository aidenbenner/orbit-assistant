#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include "sensors/sensors.h"
#include "sensors/accel.h"
#include "oled.h"
#include "serial.h"
#include "Wire_Util.h"
#include "menu.h"

#define print(x) Serial.println("x")

void device_init (){
  serial_init ();
  WireInit ();
  oled_init ();
  accel_init ();
  init_sensors (); 
  set_all_led (255);
}

void setup ()
{
  device_init ();
}

void loop ()
{
  display_test (); 
  delay(50);
}

