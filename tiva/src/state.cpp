#include <Energia.h>
#include <stdlib.h>
#include "serial.h"
#include "parser.h"
#include "state.h"

Date *g_date;
Weather *g_weather;

Weather * update_weather (Weather *weather) {
  if (weather != NULL)
    free (weather);

  Serial.println ("GET_WEATHER:NULL");
  char *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);
  char *description = get_value ("description", buffer, jb);

  Serial.println (description);

  delete_json_buffer (jb);
  free (buffer);
}
