#include <jsmn.h>

#ifndef PARSER_H
#define PARSER_H

typedef struct json_buffer {
  int len;
  jsmntok_t *t;
} json_buffer;

json_buffer * parse_json (const char *json_string);

void delete_json_buffer (json_buffer *jb);

char * get_value (char *key, const char *json_string, json_buffer *jb);

#endif
