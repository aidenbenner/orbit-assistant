#ifndef STATE_H
#define STATE_H
  
typedef struct Date {
  int minute;
  int hour;
  int second; 
  int day; 
  int month;
  int year; 
  long init_time; 
} Date;

typedef struct Weather {
  int temp;
  int high;
  double humidity;
  double precip;
  char * description; 
  char * location; 
  long init_time; 
} Weather;

typedef struct Post {
  char * title;  
  int size; 
} Post;

extern Date *g_date;
extern Weather *g_weather;

Weather * update_weather (Weather *weather);

#endif
