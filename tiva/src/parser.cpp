#include <Energia.h>
#include <string.h>
#include <stdlib.h>
#include <jsmn.h>
#include "parser.h"

static const int BUFFER_MAX = 128;

static int compare_key (const char *json, jsmntok_t *tok, const char *key)
{
	if (tok->type == JSMN_STRING &&
      (int) strlen(key) == tok->end - tok->start &&
			strncmp(json + tok->start, key, tok->end - tok->start) == 0)
		return 0;

	return -1;
}

void delete_json_buffer (json_buffer *jb)
{
  free (jb->t);
  free (jb);
}

json_buffer * parse_json (const char *json_string)
{
  json_buffer *jb = (json_buffer *) malloc (sizeof (json_buffer));
  jb->t = (jsmntok_t *) malloc (BUFFER_MAX * sizeof (jsmntok_t));

	jsmn_parser p;

	jsmn_init (&p);
  int r = jsmn_parse (&p,
      json_string,
      strlen (json_string),
      jb->t,
      BUFFER_MAX);

  jb->len = r;

  return jb;
}

char * get_value (char *key, const char *json_string, json_buffer *jb)
{
  for (int i = 1; i < jb->len; i++)
    if (compare_key(json_string, jb->t + i, key) == 0)
      return strndup (json_string + jb->t[i+1].start,
          jb->t[i+1].end - jb->t[i+1].start);

  return "NULL";
}
