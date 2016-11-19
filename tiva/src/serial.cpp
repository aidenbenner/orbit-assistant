#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"

void init_serial ()
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



