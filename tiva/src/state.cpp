#include <Arduino.h>
#include <Energia.h>
#include <stdlib.h>
#include <string.h>
#include "serial.h"
#include "parser.h"
#include "state.h"

const int NUM_POSTS = 5;
const int NUM_SUBREDDITS = 3;

User *g_user;
Date *g_date;
Weather *g_weather;
Subreddit *g_jokes;
Subreddit *g_news;
Subreddit *g_uwaterloo;
Subreddit **g_subreddits; 

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

void refresh_jokes (void)
{
  g_jokes = update_subreddit (g_jokes, "jokes");
}

void refresh_news (void)
{
  g_news = update_subreddit (g_news, "worldnews");
}

void refresh_uw (void)
{
  g_uwaterloo = update_subreddit (g_uwaterloo, "quotes");
}

void refresh_reddit (void)
{
  refresh_jokes ();
  refresh_news ();
  refresh_uw (); 
}

void refresh_subreddits (void) 
{
  refresh_reddit ();
  if(g_subreddits)
  {
    free(g_subreddits);
    for(int i = 0; i<NUM_SUBREDDITS; i++)
    {
      free(g_subreddits[i]);
    }
  }
  g_subreddits = (Subreddit **)malloc(sizeof(Subreddit**));
  for(int i = 0; i<NUM_SUBREDDITS; i++)
  {
    g_subreddits[i] = (Subreddit *)malloc (sizeof(Subreddit *));
  }
  g_subreddits[0] = g_jokes;
  g_subreddits[1] = g_news;
  Serial.println (g_subreddits[1]->name);
  g_subreddits[2] = g_uwaterloo;
}

void refresh_all (void)
{
  refresh_user ();
  refresh_date ();
  refresh_weather ();
  refresh_subreddits (); 
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

Subreddit * update_subreddit (Subreddit *subreddit, char * name)
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
  
  Serial.print ("GET_NEWS:");
  Serial.print (name);
  Serial.print (":");
  Serial.println (NUM_POSTS);
  
  char *buffer = serial_readline ();
  json_buffer *jb = parse_json (buffer);

  subreddit = (Subreddit *) malloc (sizeof (Subreddit));
  subreddit->posts = (Post *) malloc (sizeof (Post) * NUM_POSTS);
  subreddit->number = NUM_POSTS;
  subreddit->name = (char *)malloc(20 * sizeof(char));
  strcpy(subreddit->name, name);


  char **titles = get_values ("title", buffer, jb, 2);
  char **texts = get_values ("text", buffer, jb, 2);

  for (int i = 0; i < NUM_POSTS; i ++)
  {
    subreddit->posts[i].title = titles[i];
    subreddit->posts[i].text = texts[i];
  }

  return subreddit;
}

void send_email (char * to, char * body)
{
  Serial.print("SEND_MAIL:");
  Serial.print(to);
  Serial.print(":");
  Serial.println(body);
}




