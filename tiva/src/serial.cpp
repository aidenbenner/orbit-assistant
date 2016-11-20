#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"

void serial_init ()
{
  Serial.begin (9600);
  // get time
  

}

void serial_get_date_time () 
{
  Serial.println ("GET_TIME:");


}



void serial_test ()
{
  Serial.println("hello world\n");
}

void print_accel () 
{

}

void serial_get_name ()
{
  Serial.println("name:");
}

void serial_get_calendar ()
{
  Serial.println("calendar:");
}

void serial_get_weather ()
{
  Serial.println("weather:");
}

void serial_get_joke ()
{
  Serial.println("joke:5");
}

void serial_get_notifications ()
{
  Serial.println("weather:");

}


