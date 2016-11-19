#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"

void serial_init ()
{
  Serial.begin (9600);
}

void serial_test ()
{
  Serial.println("hello world\n");
}

void print_accel () 
{

}



