#include <Arduino.h>
#include <Energia.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "parser.h"
#include "state.h"

User *g_user;
Date *g_date;
Weather *g_weather;

void update_all (void)
{
  g_user = update_user (g_user);
  g_date = update_date (g_date);
  g_weather = update_weather (g_weather);
}

User * update_user (User *user)
{
  if (user != NULL)
  {
    free (user->name);
    free (user->city);
    free (user->country);
    free (user);
  }

  Serial.println ("GET_INFO:NULL");
  char *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);

  user = (User *) malloc (sizeof (User));
  user->name = get_value ("name", buffer, jb);
  user->city = get_value ("city", buffer, jb);
  user->country = get_value ("country", buffer, jb);

  delete_json_buffer (jb);
  free (buffer);

  return user;
}

Date * update_date (Date *date)
{
  if (date != NULL)
    free (date);

  Serial.println ("GET_DATE:NULL");
  char *tmp, **end, *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);
  date = (Date *) malloc (sizeof (Date));

  tmp = get_value ("second", buffer, jb);
  date->second = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("minute", buffer, jb);
  date->minute = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("hour", buffer, jb);
  date->hour = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("day", buffer, jb);
  date->day = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("month", buffer, jb);
  date->month = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("year", buffer, jb);
  date->year = strtol (tmp, end, 10);
  free (tmp);

  date->init_time = millis();

  delete_json_buffer (jb);
  free (buffer);

  return date;
}

Weather * update_weather (Weather *weather)
{
  if (weather != NULL)
  {
    free (weather->weather);
    free (weather->description);
    free (weather->temp);
    free (weather->temp_max);
    free (weather->temp_min);
    free (weather->pressure);
    free (weather->humidity);
    free (weather);
  }

  Serial.println ("GET_WEATHER:ca:waterloo");
  char *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);

  weather = (Weather *) malloc (sizeof (Weather));
  weather->weather = get_value ("weather", buffer, jb);
  weather->description = get_value ("description", buffer, jb);
  weather->temp = get_value ("temp", buffer, jb);
  weather->temp_max = get_value ("temp_max", buffer, jb);
  weather->temp_min = get_value ("temp_min", buffer, jb);
  weather->pressure = get_value ("pressure", buffer, jb);
  weather->humidity = get_value ("humidity", buffer, jb);

  delete_json_buffer (jb);
  free (buffer);

  return weather;
}
