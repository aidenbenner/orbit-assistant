#include <Arduino.h>
#include <Energia.h>
#include <string.h> 
#include <stdlib.h>
#include "serial.h"
#include "utils.h"

const int BUFFER_MAX = 256;

void serial_init ()
{
  Serial.begin (9600);
  //serial_update_weather (); 
}

//reads next line into serial_buffer
char * serial_readline (void)
{
  char *buffer = (char *) malloc (sizeof (char) * BUFFER_MAX);
  memset (buffer, '\0', BUFFER_MAX);

  char c;
  int count = 0;

  while (count < BUFFER_MAX - 1)
  {
    if (Serial.available ())
    {
      c = Serial.read();

      if (c == '\n')
        break;

      buffer[count] = c;
      count ++;
    }
  }

  return buffer;
}

/*
void serial_read_line_after_char (char n) 
{
  char c = n+1; 
  do 
  { 
    if(Serial.available())
      c = Serial.read();
  } while(c != n);
  return serial_read_line();
}

void serial_update_date () 
{
  Serial.println ("GET_TIME:NULL");

  int length = strlen(serial_buffer); 
  //needs to be replaced with actual serial data
  //placeholder for now
  char * date_str = "2016-11-25T20:13:23"; 
  g_date.year   = string_section_to_int(date_str,0,4);
  g_date.month  = string_section_to_int(date_str,5,7);
  g_date.day    = string_section_to_int(date_str,8,10);
  g_date.minute = string_section_to_int(date_str,11,13);
  g_date.hour   = string_section_to_int(date_str,14,16);
  g_date.second = string_section_to_int(date_str,17,19);
  g_date.init_time = millis();

  Serial.println(g_date.year);
  Serial.println(g_date.month);
  Serial.println(g_date.day);
  Serial.println(g_date.minute);
  Serial.println(g_date.hour);
  Serial.println(g_date.second);
}
*/
