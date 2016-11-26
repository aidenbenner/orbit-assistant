#include <Energia.h>
#include <Arduino.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitOled.h>
#include <FillPat.h>
#include <stdlib.h>
#include "string.h"
#include "sensors/accel.h"
#include "sensors/sensors.h"
#include "oled.h"
#include "utils.h"
#include "serial.h"

//pre declarations 
void debug_display_long_string_with_pot () ;
void orbit_moveto_line (int line); 
void display_menu ();
void menu_refresh ();
void display_user_prompt (const char * display_string);
int get_line_y (int line);

//external  
extern int xchOledMax; // defined in OrbitOled.c
extern int ychOledMax; // defined in OrbitOled.c
extern int xchOledCur;
extern int ychOledCur;

//defined in oled.c 
extern const int SCREEN_LENGTH;
extern const int SCREEN_HEIGHT;
extern const int INPUT_TIME_THRESH;  //ms
extern const int DELAY_MS;  //ms
extern const double CHAR_HEIGHT; 
extern const double CHAR_WIDTH; 

//constants

//local variables
static char user_name[] = "Lawrence";
static char time_buffer[CHARS_PER_LINE]; 

//placeholder for testing 
//this populates g_date and g_weather with dummy data
void test_data() 
{
  g_date.year  = 2016;
  g_date.month = 11;
  g_date.day   = 25;
  g_date.minute = 20;
  g_date.hour   = 12;
  g_date.second = 39;
  g_date.init_time = millis();
  
  g_weather.temp = 20; 
  g_weather.high = 29; 
  g_weather.description = "Cloudy and a chance of thunder."; 
  g_weather.location = "Waterloo, ON"; 
}

void menu_init ()
{
  test_data() ;
  //update the data from pi  
  menu_refresh ();
}

void menu_refresh () 
{
  //regrab data from the pi 
  //serial_update_weather (); 
  //serial_update_date (); 
}

void get_user_name ()
{
  display_user_prompt ("Please enter your name"); 
  strcpy(user_name, get_user_input());
}


void update_time () 
{
  struct Date * curr_date = serial_get_date (); 
  long delta_t = millis() - curr_date->init_time;
  if(delta_t < 1000) return;
  //convert to seconds 
  int seconds = delta_t / 1000; 
  //keep track of rounding errors
  curr_date->init_time = millis() - delta_t % 1000;
  curr_date->second += seconds;
  curr_date->minute += curr_date->second / 60;
  curr_date->hour += curr_date->minute / 60;
  curr_date->day += curr_date->hour / 24;
  curr_date->second %= 60;
  curr_date->minute %= 60;
  curr_date->hour %= 24;
  curr_date->day %= 31;
  //TODO months + years 
}

static const int NUM_MENUS = 3; 
int get_menu_selection () 
{
  double pot = read_pot_percent(); 
  return round(pot * NUM_MENUS);
}

void fill_time_buffer () 
{
  struct Date * curr_date = serial_get_date (); 
  update_time(); 
  time_buffer[0] = curr_date->hour / 10;  
  time_buffer[1] = curr_date->hour % 10;  
  time_buffer[2] = ':';  
  time_buffer[3] = curr_date->minute / 10;  
  time_buffer[4] = curr_date->minute % 10;  
  time_buffer[5] = ':';  
  time_buffer[6] = curr_date->second / 10;  
  time_buffer[7] = curr_date->second % 10;  
  for(int i = 0; i<=7; i++){
    if(i == 2 || i == 5) continue;
    time_buffer[i] += 48;
  }
}

char date_buffer[10]; 
void fill_date_buffer () 
{
  struct Date * curr_date = serial_get_date (); 
  update_time(); 
  date_buffer[0] = curr_date->day / 10;  
  date_buffer[1] = curr_date->day % 10;  
  date_buffer[2] = '/';  
  date_buffer[3] = curr_date->month/ 10;  
  date_buffer[4] = curr_date->month % 10;  
  date_buffer[5] = '/';  
  date_buffer[6] = curr_date->year / 1000 % 10;  
  date_buffer[7] = curr_date->year / 100 % 10;  
  date_buffer[8] = curr_date->year / 10 % 10;  
  date_buffer[9] = curr_date->year % 10;  
  for(int i = 0; i<=9; i++){
    if(i == 2 || i == 5) continue;
    date_buffer[i] += 48;
  }
  date_buffer[10] = 0; 
}


long last_page_time = millis(); 

//returns if the user clicks up or down to click
int get_page_action (int curr, int max)
{
  if(read_button(1))
  {
    if(millis() - last_page_time > INPUT_TIME_THRESH)
    {
      if(curr < max) ++curr;
    }
  }
  if(read_button(0))
  {
    if(millis() - last_page_time > INPUT_TIME_THRESH)
    {
      if(curr > 0) --curr;
    }
  }
  return curr; 
}

void intro_page_tick () 
{
  int init_selection = get_menu_selection();
  int scroll = 0; 
  struct Date * curr_date = serial_get_date (); 
  int init_switch = read_switch(0); 
  while(init_selection == get_menu_selection()){
    if(init_switch != read_switch(0)){
      get_user_name ();
    }
    fill_time_buffer();
    OrbitOledClearBuffer ();
    if(scroll < 1){
      orbit_moveto_line(1 - scroll);
      orbit_display_centered_string (time_buffer);
    }
    orbit_moveto_line(2 - scroll);
    if(curr_date->hour < 12)
      orbit_display_centered_string ("Good Morning ");
    else if(curr_date->hour < 18)
      orbit_display_centered_string ("Good Afternoon ");
    else 
      orbit_display_centered_string ("Good Evening ");
    orbit_moveto_line (3 - scroll);
    orbit_display_centered_string (user_name);
    if(scroll == 1){
      fill_date_buffer (); 
      orbit_moveto_line (4 - scroll);
      orbit_display_centered_string (date_buffer);
    }
    OrbitOledUpdate ();

    scroll = get_page_action(scroll,1); 
    delay (10);
  }
}

void calendar_page_tick () 
{
  OrbitOledClearBuffer ();
  orbit_moveto_line(1);
  orbit_display_centered_string ("Menu 3");
  OrbitOledUpdate ();
}

void weather_page_tick () 
{
  int init_selection = get_menu_selection();
  long init_time = millis(); 
  long init_delay= 500; 
  int tmp_size = 20;
  char temp[tmp_size]; 
  char second_line[30]; 
  int_to_char_arr (temp, tmp_size, g_weather.temp);
  strcpy (second_line, temp); 
  strcat (second_line, "C High "); 
  int_to_char_arr (temp, tmp_size, g_weather.high);
  strcat(second_line, temp); 
  strcat (second_line, "C"); 
  while(init_selection == get_menu_selection()){
    OrbitOledClearBuffer ();
    orbit_moveto_line (1);
    marquee_text (g_weather.location, init_time, init_delay); 
    orbit_moveto_line (2);
    marquee_text (second_line,init_time,init_delay);
    orbit_moveto_line (3);
    marquee_text(g_weather.description, init_time, init_delay); 
    OrbitOledUpdate ();
  }  
}

long last_switch_time = millis();
int curr_menu = 0; 

void display_menu ()
{
  switch(curr_menu)
  {
    case 0:
      intro_page_tick ();
      break;
    case 1:
      weather_page_tick ();
      break;
    case 2:
      calendar_page_tick ();
      break;
  }
  curr_menu = get_menu_selection();
} 

/**
void debug_display_long_string_with_pot () 
{
  char * input = test_string;

  int length = strlen(input);
  int num_lines = length / CHARS_PER_LINE;

  while(true)
  { 
    double percent_pot = read_pot_percent ();
    Serial.print (" POT PERCENT | ");
    Serial.print (percent_pot );
    Serial.print (" PURE ANALOG ");
    Serial.print (read_pot());
    int offset = fmod(percent_pot * num_lines * CHAR_HEIGHT, CHAR_HEIGHT);
    Serial.print (" OFFSET | ");
    Serial.print(offset);
    int min_vis_line = round(percent_pot * num_lines);
    Serial.print (" MIN_VS | ");
    Serial.println(min_vis_line);
    //need to find which three lines are visible 
    OrbitOledClear (); 
    send_line_to_buffer (input,min_vis_line);
    OrbitOledMoveTo (LINE_1_X,LINE_1_Y  - offset);
    OrbitOledDrawString (line_buffer);

    send_line_to_buffer (input,min_vis_line + 1);
    OrbitOledMoveTo (LINE_2_X,LINE_2_Y - offset);
    OrbitOledDrawString (line_buffer);

    send_line_to_buffer (input,min_vis_line + 2);
    OrbitOledMoveTo (LINE_3_X,LINE_3_Y - offset);
    OrbitOledDrawString (line_buffer);
    OrbitOledUpdate ();
    delay(50);
    //next line
  }  
} **/ 
