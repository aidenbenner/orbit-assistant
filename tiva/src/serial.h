#ifndef SERIAL_H
#define SERIAL_H
  
//structs for receiving data from pi 
typedef struct Date{
  int minute;
  int hour;
  int second; 
  int day; 
  int month;
  int year; 
  long init_time; 
} Date;

typedef struct Weather{
  int temp;
  double humidity;
  double precip;
  char * description; 
  long init_time; 
} Weather;

typedef struct Post {
  char * title;  
  int size; 
} Post;


Weather * serial_get_weather ();
Date * serial_get_date ();
void serial_init ();
void serial_update_date ();
void serial_update_weather ();
void serial_update_news ();
void serial_print_debug (char output[]);
void serial_log_err (char message[]);

#endif
