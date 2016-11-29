#include <Arduino.h>
#include <Energia.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "parser.h"
#include "state.h"

static const int NUM_POSTS = 5;
static const int NUM_SUBREDDITS = 3;
// max size of mails to get, actual value may be lower, use g_inbox->number
static const int NUM_MAILS = 5;
static char *SUBREDDITS[] = { "worldnews", "jokes", "quotes" };

User *g_user;
Date *g_date;
Weather *g_weather;
Reddit *g_reddit;
Inbox *g_inbox;

void refresh_user (void)
{
  g_user = update_user (g_user);
}

void refresh_date (void)
{
  g_date = update_date (g_date);
}

void refresh_weather (void)
{
  g_weather = update_weather (g_weather);
}

void refresh_reddit (void)
{
  g_reddit = update_reddit (g_reddit);
}

void refresh_inbox (void)
{
  g_inbox = update_inbox (g_inbox);
}

void refresh_all (void)
{
  refresh_user ();
  refresh_date ();
  refresh_weather ();
  refresh_reddit ();
  refresh_inbox ();
}

User * update_user (User *user)
{
  if (user != NULL)
  {
    free (user->name);
    free (user->city);
    free (user->country);
    free (user);
  }

  Serial.println ("GET_INFO:NULL");
  char *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);

  user = (User *) malloc (sizeof (User));
  user->name = get_value ("name", buffer, jb);
  user->city = get_value ("city", buffer, jb);
  user->country = get_value ("country", buffer, jb);

  delete_json_buffer (jb);
  free (buffer);

  return user;
}

Date * update_date (Date *date)
{
  if (date != NULL)
    free (date);

  Serial.println ("GET_DATE:NULL");
  char *tmp, **end, *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);
  date = (Date *) malloc (sizeof (Date));

  tmp = get_value ("second", buffer, jb);
  date->second = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("minute", buffer, jb);
  date->minute = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("hour", buffer, jb);
  date->hour = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("day", buffer, jb);
  date->day = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("month", buffer, jb);
  date->month = strtol (tmp, end, 10);
  free (tmp);

  tmp = get_value ("year", buffer, jb);
  date->year = strtol (tmp, end, 10);
  free (tmp);

  date->init_time = millis();

  delete_json_buffer (jb);
  free (buffer);

  return date;
}

Weather * update_weather (Weather *weather)
{
  if (weather != NULL)
  {
    free (weather->weather);
    free (weather->description);
    free (weather->temp);
    free (weather->temp_max);
    free (weather->temp_min);
    free (weather->pressure);
    free (weather->humidity);
    free (weather);
  }

  Serial.println ("GET_WEATHER:ca:waterloo");
  char *buffer = serial_readline ();

  json_buffer *jb = parse_json (buffer);

  weather = (Weather *) malloc (sizeof (Weather));
  weather->weather = get_value ("weather", buffer, jb);
  weather->description = get_value ("description", buffer, jb);
  weather->temp = get_value ("temp", buffer, jb);
  weather->temp_max = get_value ("temp_max", buffer, jb);
  weather->temp_min = get_value ("temp_min", buffer, jb);
  weather->pressure = get_value ("pressure", buffer, jb);
  weather->humidity = get_value ("humidity", buffer, jb);

  delete_json_buffer (jb);
  free (buffer);

  return weather;
}

void free_subreddit (Subreddit *subreddit)
{
  if (subreddit)
  {
    for (int i = 0; i < subreddit->number; i ++)
    {
      free (subreddit->posts[i].title);
      free (subreddit->posts[i].text);
    }
    free (subreddit->posts);
    free (subreddit->name);
    free (subreddit);
  }
}

Subreddit * update_subreddit (Subreddit *subreddit, char *name)
{
  free_subreddit (subreddit);

  Serial.print ("GET_NEWS:");
  Serial.print (name);
  Serial.print (":");
  Serial.println (NUM_POSTS);

  char *buffer = serial_readline ();
  json_buffer *jb = parse_json (buffer);

  subreddit = (Subreddit *) malloc (sizeof (Subreddit));
  subreddit->posts = (Post *) malloc (sizeof (Post) * NUM_POSTS);
  subreddit->number = NUM_POSTS;
  subreddit->name = strdup (name);

  char **titles = get_values ("title", buffer, jb, 2);
  char **texts = get_values ("text", buffer, jb, 2);

  for (int i = 0; i < NUM_POSTS; i ++)
  {
    subreddit->posts[i].title = titles[i];
    subreddit->posts[i].text = texts[i];
  }

  delete_json_buffer (jb);
  free (titles);
  free (texts);
  free (buffer);

  return subreddit;
}

Reddit * update_reddit (Reddit *reddit)
{
  if (reddit)
  {
    for (int i = 0; i < reddit->number; i ++)
      free_subreddit (reddit->subreddits[i]);
    free (reddit);
  }

  reddit = (Reddit *) malloc (sizeof (Reddit));
  reddit->number = 3;
  reddit->subreddits = (Subreddit **) malloc (NUM_SUBREDDITS * sizeof (Subreddit *));

  for (int i = 0; i < NUM_SUBREDDITS; i ++)
    reddit->subreddits[i] = update_subreddit ((Subreddit *) NULL, SUBREDDITS[i]);

  return reddit;
}

void free_mail (Mail *mail)
{
  if (mail)
  {
    free (mail->to);
    free (mail->from);
    free (mail->subject);
    free (mail->body);
    free (mail);
  }
}

Inbox * update_inbox (Inbox *inbox)
{
  if (inbox)
  {
    for (int i = 0; i < inbox->number; i ++)
      free_mail (inbox->mails[i]);
    free (inbox);
  }

  Serial.print ("GET_MAILS:");
  Serial.println (NUM_MAILS);

  char *buffer = serial_readline ();
  json_buffer *jb = parse_json (buffer);

  inbox = (Inbox *) malloc (sizeof (Inbox));

  char **froms = get_values ("from", buffer, jb, 3);
  char **subjects = get_values ("subject", buffer, jb, 3);
  char **bodies = get_values ("body", buffer, jb, 3);
  char **tos = get_values ("to", buffer, jb, 3);

  int len = jb->t[0].size;
  inbox->number = len;

  for (int i = 0; i < len; i ++)
  {
    inbox->mails[i]->from = froms[i];
    inbox->mails[i]->subject = subjects[i];
    inbox->mails[i]->body = bodies[i];
    inbox->mails[i]->to = tos[i];
  }

  delete_json_buffer (jb);
  free (froms);
  free (subjects);
  free (bodies);
  free (tos);
  free (buffer);

  return inbox;
}

void send_email (char * to, char * body)
{
  Serial.print("SEND_MAIL:");
  Serial.print(to);
  Serial.print(":");
  Serial.println(body);
}
