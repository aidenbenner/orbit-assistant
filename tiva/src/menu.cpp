
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
#include "state.h"

//pre declarations
void debug_display_long_string_with_pot () ;
void orbit_moveto_line (int line);
void display_menu ();
void menu_refresh ();
void menu_tick ();
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
const int NUM_MAIL_MESSAGES = 5; 

//static constants
static const int MARQUEE_DELAY = 500; //ms
static const int NUM_MENUS = 4;

//static variables
static int curr_menu = 0;
static int NUM_UNREAD_MAIL = 5;
static char time_buffer[CHARS_PER_LINE];
static char date_buffer[CHARS_PER_LINE];
static long last_menu_switch_time = millis();
static long last_pot_move_time = millis();
static long top_bar_time_thresh = 800;
static long last_page_action_time = millis();
static double last_pot_val = 0;


/**
 * Prompts and gets the username from the user through the Oled 
 * Updates g_user with the new name 
 **/
void get_user_name ()
{
  display_user_prompt ("Please enter your name");
  char name[30];
  strcpy(name, get_user_input());
  g_user = update_user (g_user, "name", name);
}

/**
 * updates the values of g_date using the difference between the
 * current clock time and the initial clock time 
 **/ 
void update_time ()
{
  Date * curr_date = g_date;
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

/**
 * Gets the currently selected menu using the potentionmeter
 **/ 
int get_menu_selection ()
{
  double pot = read_pot_percent();
  int out = (pot * NUM_MENUS);
  if(out == NUM_MENUS) out--;
  return out;
}

/**
 * Converts g_date into a string and reads it into the time_buffer static varibale
 **/ 
void fill_time_buffer ()
{
  Date * curr_date = g_date;
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

/** 
 * Reads the g_date global into the static variable date_buffer
 **/ 
void fill_date_buffer ()
{
  struct Date * curr_date = g_date;
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

/**
 * returns 1 if the user clicks up after the input_time_thresh
 * returns -1 if the user clicks down after the input_time_thresh
 **/ 
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

/**
 * code that runs when the user is on the intro page
 * displays the user's name, time, and date
 **/ 
void intro_page_tick (int selection)
{
  int init_selection = get_menu_selection();
  int scroll = 0;
  Date * curr_date = g_date;
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

    //print name
    orbit_moveto_line (3 - scroll);
    orbit_display_centered_string (g_user->name);
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

/**
 * Code that runs when user selects a reddit post from the news page
 * Allows the user to scroll up and down to read the post
 * and displays it in a paginated form 
 **/ 
void view_news_page (int selection, Post article)
{
  int tog = read_switch(0);
  int page = 0;
  int len = strlen (article.text);
  bool title = false;
  if(len < 5){
    title = true;
    len = strlen (article.title);
  }
  int page_max = len / CHARS_PER_LINE - 1;

  while(tog == read_switch(0) && selection == get_menu_selection())
  {
    OrbitOledClearBuffer();
    oled_paint_progress_bar(page,page_max);
    if(title)
    {
      print_string_page(article.title, page);
    }
    else
    {
      print_string_page(article.text, page);
    }
    page = get_page_action (page, page_max);
    OrbitOledUpdate();
  }
}

/**
 * Prompt the user to enter an email reply and send it 
 **/ 
void reply_message(int selection, Mail * message)
{
  int tog = read_switch(0);
  display_user_prompt("Enter a reply ");
  char reply_buffer[1000];
  while(tog == read_switch(0) && selection == get_menu_selection()){
    char subject[100];
    strcpy(subject, "re ");
    strcat(subject, message->subject);
    send_email (message->from, subject, get_user_input());
  }
}

void view_mail_message (int selection, Mail * message)
{
  int tog = read_switch(0);
  int tog2 = read_switch(1);
  int page = 0;
  //this is so slow... each cat is O(n) I don't think it's that big a deal
  int body_len = strlen(message->body);
  long init_time = millis();
  long delay = 500;
  int line_select = 1;
  int top_line = 0;
  char line_buf[CHARS_PER_LINE * 5];
  int page_max = body_len / CHARS_PER_LINE + 3;
  long time_selected_init = millis();
  while(tog == read_switch(0) && selection == get_menu_selection())
  {
    OrbitOledClearBuffer();
    if(tog2 != read_switch(1)){
      reply_message(selection, message);
    }

    strcpy(line_buf, "From: ");
    strcat(line_buf, message->from);
    marquee_text_if_selected (line_buf ,init_time,delay, line_select == 1, 1 - page);

    strcpy(line_buf, "To: ");
    strcat(line_buf, message->to);
    marquee_text_if_selected (line_buf,init_time,delay, line_select == 2, 2 - page);

    strcpy(line_buf, "Subject: ");
    strcat(line_buf, message->subject);
    marquee_text_if_selected (line_buf, init_time,delay, line_select == 3, 3 - page);

    oled_draw_multiline_string (message->body, 4 - page);
    //page = get_page_action (page, 3 + body_len / CHARS_PER_LINE - 2);
    oled_paint_line_selection (line_select);
    //page min = 0

    int new_line_select = get_page_action (line_select, 4);
    if(new_line_select != line_select){
      init_time = millis();
      if(new_line_select >= 4){
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
    OrbitOledUpdate();
  }
}

/**
 * Code that runs when user is on the News/Reddit page 
 * displays several different subreddits and allows the user to toggle between them 
 **/ 
void news_page_tick (int selection)
{
  int init_selection = get_menu_selection();
  long init_time = millis();
  long time_selected_init = millis();
  long marquee_delay = MARQUEE_DELAY;
  int line_select = 1;
  int page = 0 ;
  int page_max = g_reddit->subreddits[0]->number - 2;
  int init_toggle = read_switch(0);
  int tog2 = read_switch(1);
  int sub_ind = 0;
  while(selection == get_menu_selection()){
    OrbitOledClearBuffer ();

    if(tog2 != read_switch(1)){
      tog2 = read_switch(1);
      sub_ind++;
      sub_ind = sub_ind % NUM_SUBREDDITS;
    }

    //page is article we start with
    //repeat for each line
    for(int i = 1; i<=3; i++){
      orbit_moveto_line(i);
      if(0 == page + i - 1){
        char tmp[30];
        strcpy(tmp,"Reddit: ");
        strcat(tmp,g_reddit->subreddits[sub_ind]->name);
        if(line_select == 1){
          marquee_text (tmp, time_selected_init, marquee_delay);
        }
        else {
          OrbitOledDrawString (tmp);
        }
        continue;
      }
      if(page + i - 2 > g_reddit->subreddits[sub_ind]->number){
        continue;
      }
      if(line_select == i){
        marquee_text (g_reddit->subreddits[sub_ind]->posts[page +  i - 2].title, time_selected_init, marquee_delay);
      }
      else{
        OrbitOledDrawString (g_reddit->subreddits[sub_ind]->posts[page + i - 2].title);
      }
    }
    oled_paint_progress_bar(page,page_max);
    oled_paint_line_selection (line_select);
    menu_tick ();
    OrbitOledUpdate ();
    if(init_toggle != read_switch(0)){
      if(!(page == 0 && line_select == 1))
      {
        view_news_page (selection, g_reddit->subreddits[sub_ind]->posts[page + line_select - 2]);
      }
    }
    int new_line_select = get_page_action (line_select, 4);
    if(new_line_select != line_select){
      if(new_line_select >= 4){
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

/**
 * this code is called on every screen to continuously update the 
 * indicator leds and also the top progress bar whenever the user
 * moves the pot
 **/ 
void menu_tick ()
{
  if(fabs(last_pot_val - read_pot_percent()) > 0.01){
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
  long time_selected_init = millis();
  long init_time = millis();

  //TODO maybe get rid of these magic numbers
  long init_delay= 700;
  int tmp_size = 20;
  char temp[tmp_size];
  //need to convert our weather struct into a printable string int page = 0; 
  int page_max = 5;
  int line_select = 1;
  char second_line[30];
  strcpy (second_line, g_weather->temp);
  strcat (second_line, "C H:");
  strcat(second_line, g_weather->temp_max);
  strcat (second_line, "C L:");
  strcat(second_line, g_weather->temp_min);
  strcat (second_line, "C");

  char humidity_str[30];
  strcpy (humidity_str, "Humidity ");
  strcat (humidity_str, g_weather->humidity);
  strcat (humidity_str, "%");

  char pressure_str[30];
  strcpy (pressure_str, "Pressure ");
  strcat (pressure_str, g_weather->pressure);
  strcat (pressure_str, " Torr");

  int page = 0; 
  while(selection == get_menu_selection()){
    OrbitOledClearBuffer ();

    int curr_line = 1 - page;
    marquee_text_if_selected (g_user->city, time_selected_init, init_delay,
       1 == line_select + page, curr_line++);
    marquee_text_if_selected (second_line, time_selected_init, init_delay,
       2 == line_select + page, curr_line++);
    marquee_text_if_selected (g_weather->description, time_selected_init, init_delay,
       3 == line_select + page, curr_line++);
    marquee_text_if_selected (humidity_str, time_selected_init, init_delay,
       4 == line_select + page, curr_line++);
    marquee_text_if_selected (pressure_str, time_selected_init, init_delay,
       5 == line_select + page, curr_line++);

    oled_paint_line_selection (line_select);
    oled_paint_progress_bar(page, page_max);

    menu_tick ();
    OrbitOledUpdate ();

    int new_line_select = get_page_action (line_select, 4);
    if(new_line_select != line_select)
    {
      if(new_line_select >= 4)
      {
        if(page < page_max)
        {
          page++;
          line_select--;
        }
        new_line_select = 3;
        line_select = 3;
        time_selected_init = millis();
      }
      if(new_line_select <= 0)
      {
        if(page > 0)
        {
          page--;
          line_select++;
        }
        line_select = 1;
        new_line_select = 1;
        time_selected_init = millis();
      }
      else
      {
        time_selected_init = millis();
        line_select = new_line_select;
      }
    }
  }
}

/**
 * code that runs when the user selects the mail page
 * shows the current number of unread messages
 * and lists each message allowing the user to scroll up and down
 * read and respond to message
 **/ 
void mail_page_tick(int selection)
{
  int init_selection = get_menu_selection();
  long init_time = millis();
  long time_selected_init = millis();
  long marquee_delay = 500;

  int line_select = 1;
  int page = 0 ;
  int page_max = NUM_MAIL_MESSAGES;
  int init_toggle = read_switch(0);
  int init_toggle2 = read_switch(1); 
  while(selection == get_menu_selection())
  {
    OrbitOledClearBuffer ();
    //page is article we start with
    //repeat for each line
    char buffer[300];
    for(int i = 1; i<=3; i++)
    {
      orbit_moveto_line(i);
      if(0 == page + i - 1)
      {
        sprintf(buffer, "Mail: Unread %d", NUM_UNREAD_MAIL);
        OrbitOledDrawString (buffer);
        continue;
      }
      if(page + i - 2 >= NUM_MAIL_MESSAGES){
        continue;
      }
      strcpy(buffer, g_inbox->mails[page+i-2]->read ? "- " : "o ");
      strcat(buffer, g_inbox->mails[page+i-2]->subject);
      strcat(buffer, " | ");
      strcat(buffer, g_inbox->mails[page+i-2]->from);
      //draw checkbox
      if(line_select == i)
      {
        marquee_text (buffer, time_selected_init, marquee_delay);
      }
      else
      {
        OrbitOledDrawString (buffer);
      }
    }
    oled_paint_progress_bar(page,page_max);
    oled_paint_line_selection (line_select);
    menu_tick ();
    OrbitOledUpdate ();
    if(init_toggle != read_switch(0))
    {
      view_mail_message (selection, g_inbox->mails[page + line_select - 2]);
      g_inbox->mails[page + line_select - 2]->read = true;
      NUM_UNREAD_MAIL--;
    }
    if(init_toggle2 != read_switch(1))
    {
      init_toggle2 = read_switch(1); 
      refresh_inbox(); 
    }
    int new_line_select = get_page_action (line_select, 4);
    if(new_line_select != line_select)
    {
      if(new_line_select >= 4)
      {
        if(page < page_max)
        {
          page++;
          line_select--;
        }
        new_line_select = 3;
        line_select = 3;
        time_selected_init = millis();
      }
      if(new_line_select <= 0)
      {
        if(page > 0)
        {
          page--;
          line_select++;
        }
        line_select = 1;
        new_line_select = 1;
        time_selected_init = millis();
      }
      else
      {
        time_selected_init = millis();
        line_select = new_line_select;
      }
    }
  }
}

/**
 * handles switching between the different screens
 **/ 
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
    case 3:
      mail_page_tick (3);
      break;
    default:
      break;
  }
  menu_tick ();
  curr_menu = get_menu_selection();
}

