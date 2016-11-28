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
  char *title;
  char *text;
} Post;

typedef struct Subreddit {
  char * name; 
  Post *posts;
  int number;
} Subreddit;

extern User *g_user;
extern Date *g_date;
extern Weather *g_weather;
extern Subreddit *g_jokes;
extern Subreddit *g_news;
extern Subreddit *g_uwaterloo;
extern Subreddit **g_subreddits;

extern const int NUM_SUBREDDITS; 

User * update_user (User *user);
Date * update_date (Date *date);
Weather * update_weather (Weather *weather);
Subreddit * update_subreddit (Subreddit *subreddit, char *name);

void refresh_user (void);
void refresh_date (void);
void refresh_weather (void);
void refresh_jokes (void);
void refresh_all (void);
void send_email (char * to, char * body);

#endif
