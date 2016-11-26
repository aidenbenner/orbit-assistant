#include <Energia.h>
#include <Arduino.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitOled.h>
#include <FillPat.h>
#include <stdlib.h>
#include <cmath>
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
void menu_tick ();
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
extern const unsigned int INPUT_TIME_THRESH;  //ms
extern const int DELAY_MS;  //ms
extern const double CHAR_HEIGHT; 
extern const double CHAR_WIDTH; 



//constants
static const int NUM_MENUS = 4; 

//local variables
static char user_name[] = "Lawrence";
static char time_buffer[CHARS_PER_LINE]; 
static char date_buffer[CHARS_PER_LINE]; 
static long last_menu_switch_time = millis();
static int curr_menu = 0; 

static long last_page_action_time = millis(); 

//placeholder for testing 
//this populates g_date and g_weather with dummy data

const int NUM_REDDIT_POSTS = 5; 
struct Post reddit_news[NUM_REDDIT_POSTS]; 

//DUMMY DATA TEST ONLY

  char dummy_headline_1[] = "1. Edward Snowden's bid to guarantee that he would not be extradited to the US if he visited Norway has been rejected by the Norwedgian supreme court.";
  char dummy_headline_2[] = "2. Catholic Church Finally Apologizes for Its Role in the Deaths of Over 800K During Rwandan Genocide";
  char dummy_headline_3[] = "3. Top scientist who discovered Litvinenko poison 'stabbed himself to death with two knives' after trip to Russia";
  char dummy_headline_4[] = "4. Uganda is shutting down schools funded by Mark Zuckerberg, Bill Gates ";
  char dummy_headline_5[] = "5. Google is warning prominent journalists and professors that nation-sponsored hackers have recently targeted their accounts, according to reports delivered in the past 24 hours over social media";
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

  reddit_news[0].title = dummy_headline_1;
  reddit_news[1].title = dummy_headline_2;
  reddit_news[2].title = dummy_headline_3;
  reddit_news[3].title = dummy_headline_4;
  reddit_news[4].title = dummy_headline_5;

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

int get_menu_selection () 
{
  double pot = read_pot_percent(); 
  return (pot * NUM_MENUS);
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

//returns if the user clicks up or down to click
int get_page_action (int curr, int max)
{
  if(read_button(1))
  {
    if(millis() - last_page_action_time > INPUT_TIME_THRESH)
    {
      if(curr < max) ++curr;
      last_page_action_time = millis();
    }
  }
  if(read_button(0))
  {
    if(millis() - last_page_action_time > INPUT_TIME_THRESH)
    {
      if(curr > 0) --curr;
      last_page_action_time = millis();
    }
  }
  return curr; 
}

void intro_page_tick (int selection) 
{
  int init_selection = get_menu_selection();
  int scroll = 0; 
  struct Date * curr_date = serial_get_date (); 
  int init_switch = read_switch(0); 
  while(selection == get_menu_selection()){
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
    menu_tick ();
    OrbitOledUpdate ();

    scroll = get_page_action(scroll,1); 
  }
}

void news_page_tick (int selection) 
{
  int init_selection = get_menu_selection();
  long init_time = millis(); 
  long time_selected_init = millis(); 
  long marquee_delay = 200; 

  int line_select = 1; 
  int page = 0 ; 
  int page_max = NUM_REDDIT_POSTS; 
  while(selection == get_menu_selection()){
    OrbitOledClearBuffer ();
    //page is article we start with
    //repeat for each line
    Serial.println(line_select);
    for(int i = 1; i<=3; i++){
      orbit_moveto_line(i);
      if(0 == page + i - 1){
        OrbitOledDrawString ("News: ");
        continue;
      }
      if(line_select == i){
        marquee_text (reddit_news[page +  i - 2].title, time_selected_init, marquee_delay); 
      }
      else{
        //kind of a hack
        OrbitOledDrawString (reddit_news[page + i - 2].title);
      }
    }
    oled_paint_progress_bar(page,page_max); 
    oled_paint_line_selection (line_select);
    menu_tick ();
    OrbitOledUpdate ();
    int new_line_select = get_page_action (line_select, 4); 
    if(new_line_select != line_select){
      if(new_line_select >= 4){
        Serial.println("HIT");
        if(page < page_max)
        {  
          page++;
          line_select--;
        }
        new_line_select = 3;
        line_select = 3; 
        time_selected_init = millis(); 
      }
      if(new_line_select <= 0){
        if(page > 0){
          page--; 
          line_select++; 
        } 
        line_select = 1; 
        new_line_select = 1;
        time_selected_init = millis(); 
      }
      else{
        time_selected_init = millis(); 
        line_select = new_line_select;
      }
    }
  }
}

long last_pot_move_time = millis(); 
double last_pot_val = read_pot_percent(); 
long top_bar_time_thresh = 800; 
void menu_tick () 
{
  if(fabs(last_pot_val - read_pot_percent()) > 0.01){
    Serial.println(last_pot_val);
    Serial.println(read_pot_percent());
    last_pot_val = read_pot_percent(); 
    last_pot_move_time = millis(); 
  }
  if(millis() - last_pot_move_time < top_bar_time_thresh){
    oled_paint_top_progress_bar (read_pot_percent(), 1,NUM_MENUS);
    led_encode_percent(read_pot_percent(), 1.0);
  }
  else{
    led_gradient(3000,0);
  }
}


//update the weather page 
void weather_page_tick (int selection) 
{
  int init_selection = get_menu_selection();
  long init_time = millis(); 

  //TODO maybe get rid of these magic numbers
  long init_delay= 500; 
  int tmp_size = 20;
  char temp[tmp_size]; 
  //need to convert our weather struct into a printable string
  char second_line[30]; 
  int_to_char_arr (temp, tmp_size, g_weather.temp);
  strcpy (second_line, temp); 
  strcat (second_line, "C High "); 
  int_to_char_arr (temp, tmp_size, g_weather.high);
  strcat(second_line, temp); 
  strcat (second_line, "C"); 
  while(selection == get_menu_selection()){
    OrbitOledClearBuffer ();
    orbit_moveto_line (1);
    marquee_text (g_weather.location, init_time, init_delay); 
    orbit_moveto_line (2);
    marquee_text (second_line,init_time,init_delay);
    orbit_moveto_line (3);
    marquee_text(g_weather.description, init_time, init_delay); 
    menu_tick ();
    OrbitOledUpdate ();
  }  
}

void display_menu ()
{
  switch(curr_menu)
  {
    case 0:
      intro_page_tick (0);
      break;
    case 1:
      weather_page_tick (1);
      break;
    case 2:
      news_page_tick (2);
      break;
    default:
      OrbitOledClearBuffer ();
      OrbitOledUpdate ();
      break;
  }
  menu_tick ();
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
