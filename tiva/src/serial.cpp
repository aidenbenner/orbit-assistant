#include <Arduino.h>
#include <Energia.h>
#include "sensors/accel.h"
#include "serial.h"
#include "utils.h"
#include <string> 

//pre definitions 
//TODO FIX PRINT_DEBUG PRINT
void serial_print_debug (const char * output);
void serial_print_err (const char output[]);
void serial_print_err (long output);

typedef struct Date Date; 
typedef struct Weather Weather; 

const int NUM_NEWS_POSTS = 10; 
Post news[NUM_NEWS_POSTS]; 

Date * serial_get_date () 
{
  return &g_date; 
}

Weather * serial_get_weather () 
{
  return &g_weather; 
}

void serial_init ()
{
  Serial.begin (9600);
  serial_update_date (); 
  //serial_update_weather (); 
}

static const int SERIAL_BUFF_SIZE = 500;  
static char serial_buffer[SERIAL_BUFF_SIZE]; 
//reads next line into serial_buffer
void serial_read_line () 
{
  char c = ' '; 
  char last = ' ';
  int bufInd = 0; 
  while(c != '\n')
  {
    if(bufInd >= SERIAL_BUFF_SIZE)
    {
      serial_print_err("Serial buffer overflow");
      break;
    }
    if(Serial.available())
    {
      c = Serial.read();
      if(c == '\r' && last == '\n')
      {
        serial_buffer[bufInd] = '\0'; 
        break;
      }
      serial_buffer[bufInd] = c; 
      bufInd++; 
    }
    last = c; 
  } 
  delay(20); 
}

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

/** this was for the old json api
aJsonObject * get_next_payload () 
{
  serial_read_line();
  aJsonObject * json_root = aJson.parse(serial_buffer);
  aJsonObject * time_str  = aJson.getObjectItem(jsonObject, "payload");
} **/ 

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

void serial_update_weather () 
{
  Serial.println ("GET_WEATHER:NULL");
  serial_read_line ();
  //parse at
}

void serial_update_news () 
{
  Serial.println ("GET_WEATHER:NULL");
  //thinking we could memcpy then replace delimeters with \0 
  //then have pointers to the start of each post. 
  serial_read_line ();
  //parse at
}

void serial_print_err (long output)
{
  Serial.print("PRINT_ERROR:"); 
  Serial.println(output); 
}

void serial_print_err (int output)
{
  serial_print_err((long)output); 
}

void serial_print_err (const char output[])
{
  Serial.print("PRINT_ERROR:"); 
  Serial.println(output); 
}

void serial_print_debug (const char output[])
{
  Serial.print("PRINT_DEBUG:" ); 
  Serial.println(output); 
}


