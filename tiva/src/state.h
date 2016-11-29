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

typedef struct Subreddit {
  char *name;
  Post *posts;
  int number;
} Subreddit;

typedef struct Reddit {
  Subreddit **subreddits;
  int number;
} Reddit;

typedef enum sr_type {
  NEWS,
  JOKES,
  QUOTES
} sr_type;

typedef struct Mail {
  char *to;
  char *from;
  char *subject;
  char *body;
  bool read;
  Date received;
} Mail;

typedef struct Inbox {
  Mail **mails;
  int number;
} Inbox;

extern User *g_user;
extern Date *g_date;
extern Weather *g_weather;
extern Reddit *g_reddit;
extern Inbox *g_inbox;

User * update_user (User *user);
Date * update_date (Date *date);
Weather * update_weather (Weather *weather);
Reddit * update_reddit (Reddit *reddit);
Inbox * update_inbox (Inbox inbox);

void refresh_user (void);
void refresh_date (void);
void refresh_weather (void);
void refresh_all (void);
void send_email (char * to, char * body);

#endif
