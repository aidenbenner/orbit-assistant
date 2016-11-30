#include <Energia.h>
#include <stdlib.h>
#include "sensors/sensors.h"
#include "sensors/accel.h"
#include "oled.h"
#include "serial.h"
#include "Wire_Util.h"
#include "menu.h"
#include "parser.h"
#include "state.h"

long g_device_init_time; 

void device_init ()
{
  g_device_init_time = millis(); 

  serial_init ();
  oled_init ();
  init_sensors (); 
  refresh_all ();

  led_left_right (255, true);
  led_left_right (255, false);
}

void setup ()
{
  device_init ();
}

void loop ()
{
  display_menu (); 
  delay(5);
}
