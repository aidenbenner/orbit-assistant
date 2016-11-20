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
Date curr_date;

typedef struct Weather{
  int temp;
  double humidity;
  double precip;
  long init_time; 
} Weather;
Weather curr_weather;

void serial_init ();
void serial_test ();


#endif
