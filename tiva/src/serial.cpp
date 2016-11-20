#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"

Date curr_date;
Weather curr_weather;

void serial_init ()
{
  Serial.begin (9600);
  // get time
}

const int SERIAL_BUFF_SIZE = 500;  
char serial_buffer[SERIAL_BUFF_SIZE]; 
//reads next line into serial_buffer
void serial_read_line () 
{
  char c = ' '; 
  while(c != '\n'){
    if(bufInd >= SERIAL_BUFF_SIZE){
      serial_log_error("Serial buffer overflow");
      break;
    }
    int bufInd = 0; 
    if(Serial.isAvailible() ){
      c = Serial.read();
      if(c == '\n'){
        serial_buffer[bufInd] = '\0'; 
        break;
      }
      serial_buffer[bufInd] = c; 
    }
  } 
  delay(20); 
}


void serial_get_date_time () 
{
  Serial.println ("GET_TIME:NULL");
  serial_read_line ();
  //parse at


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
void serial_print_err (char output[])
{
  Serial.print("PRINT_ERROR:"); 
  Serial.println(output); 
}

void serial_print_debug (char output[])
{
  Serial.print("DEBUG_PRINT:" ); 
  Serial.println(output); 
}


