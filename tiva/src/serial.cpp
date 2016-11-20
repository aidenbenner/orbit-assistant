#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"

char serial_buffer[500]; 
void serial_init ()
{
  Serial.begin (9600);
  // get time
  

}

void serial_read_line () 
{
  char c = ' '; 
  int bufInd = 0; 
  while(Serial.isAvailible() ){
    IncomingByte = Serial.read();

		// say what you got:
		Serial.print("I received: ");
		Serial.println(incomingByte, DEC); 
  }
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

void serial_print_debug (char * output)
{
  Serial.print("DEBUG_PRINT:" ); 
  Serial.println(output); 
}


