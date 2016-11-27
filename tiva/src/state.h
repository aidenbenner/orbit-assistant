#ifndef STATE_H
#define STATE_H

typedef struct User {
  char *name;
  char *city;
  char *country;
} User;
  
typedef struct Date {
  int second;
  int minute;
  int hour;
  int day; 
  int month;
  int year; 
  long init_time;
} Date;

typedef struct Weather {
  char *temp;
  char *temp_max;
  char *temp_min;
  char *pressure;
  char *humidity;
  char *weather;
  char *description; 
  char *location; 
} Weather;

typedef struct Mail {
  char * to;
  char * from;
  char * subject;
  char * body;
  bool read;
  Date received;
} Mail;

typedef struct Post {
  char * title;  
  int size; 
} Post;

extern User *g_user;
extern Date *g_date;
extern Weather *g_weather;

User * update_user (User *user);
Date * update_date (Date *date);
Weather * update_weather (Weather *weather);

void update_all (void);

#endif
