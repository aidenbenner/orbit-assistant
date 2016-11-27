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

typedef struct Post {
  char *title;
  char *text;
} Post;

typedef struct Posts {
  Post *post;
  int number;
} Posts;

extern User *g_user;
extern Date *g_date;
extern Weather *g_weather;
extern Posts *g_posts;

User * update_user (User *user);
Date * update_date (Date *date);
Weather * update_weather (Weather *weather);
Posts * update_posts (Posts *posts);

void refresh_user (void);
void refresh_date (void);
void refresh_weather (void);
void refresh_posts (void);
void refresh_all (void);

#endif
