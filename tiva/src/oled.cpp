#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <FillPat.h>
#include <Energia.h>
#include "sensors/sensors.h"
#include "oled.h"

//constants
const int SCREEN_LENGTH = 132;
const int SCREEN_HEIGHT = 32;
const unsigned int INPUT_TIME_THRESH = 200;  //ms
const int DELAY_MS = 50;  //m

const double CHAR_HEIGHT = 7;
const double CHAR_WIDTH = 8;

const int LINE_HEIGHT = 10;
const int LINE_TOP_OFFSET = 3;

const int LINE_1_Y = LINE_TOP_OFFSET;
const int LINE_1_X = 0;

const int LINE_2_Y = LINE_1_Y + LINE_HEIGHT;
const int LINE_2_X = 0;

const int LINE_3_Y = LINE_2_Y + LINE_HEIGHT;
const int LINE_3_X = 0;

const int CENTERED_STR_Y = 0;
const int PAGE_LINE_SHIFT = 1;

const int MARQUEE_MS_PER_CHAR = 250;

static int orbit_y = 0;
static int orbit_x = 0;

//local variables
static const char selection_string [] = "abcdefghijklmnopqrstuvwxyz .@       ";
static const char upper_selection_string [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .@       ";

static const int insert_time_thresh = 200;  //ms
static const int max_user_input_buffer = CHARS_PER_LINE * 3;
static char user_input_buffer[max_user_input_buffer * 3];


void oled_init ()
{
  OrbitOledInit ();
  OrbitOledClear ();
  OrbitOledClearBuffer ();
  OrbitOledSetFillPattern (OrbitOledGetStdPattern (iptnSolid));
  OrbitOledSetDrawMode (modOledSet);
  OrbitOledSetCharUpdate (0);
}

static char line_buffer [CHARS_PER_LINE + 1] = " ";

/**
 * copies the ith line of the input into line_buffer
 * if
 **/
static char * send_line_to_buffer (char * input, int line)
{
  int max_ind = 0;
  int len = strlen(input);
  for(int i = 0; i < CHARS_PER_LINE; i++ )
  {
    if(!input[i]) break;
    if(i + line * CHARS_PER_LINE >= len){
      line_buffer[i] = ' ';
    }
    else{
      line_buffer[i] = input[i + line * CHARS_PER_LINE];
      max_ind++;
    }
  }
  line_buffer[max_ind] = '\0';
  return line_buffer;
}

static char * send_line_to_buffer (const char * input, int line) {
  send_line_to_buffer((char *)input,line);
}

void print_string_page ( char * input, int page)
{
  int curr_line = page * PAGE_LINE_SHIFT;
  int len = strlen(input);

  send_line_to_buffer (input,curr_line);
  OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
  OrbitOledDrawString (line_buffer);

  send_line_to_buffer (input,curr_line + 1);
  OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
  OrbitOledDrawString (line_buffer);

  send_line_to_buffer (input,curr_line + 2);
  OrbitOledMoveTo (LINE_3_X,LINE_3_Y);
  OrbitOledDrawString (line_buffer);

}

void oled_draw_multiline_string(char * input, int current_line)
{
  int len = strlen(input);
  int num_input_lines = len / CHARS_PER_LINE + 1;
  //drawing current_line, n + 1 , n + 2
  //see what lines overlap with start_line

  for(int i = current_line; i<=3; i++)
  {
    if(i < 1) continue;
    orbit_moveto_line (i);
    int current_str_line = i - current_line;
    if(current_str_line < num_input_lines){
      send_line_to_buffer (input,current_str_line);
      OrbitOledDrawString (line_buffer);

    }
  }
}

void oled_paint_progress_bar (double current, double max)
{
  OrbitOledMoveTo(SCREEN_LENGTH- 1,0);
  OrbitOledDrawRect(SCREEN_LENGTH - 1, current / max * SCREEN_HEIGHT);
}


void oled_paint_top_progress_bar(double current, double max, int divisions)
{
  int y = 0;
  OrbitOledMoveTo(0,y);
  OrbitOledDrawRect(current / max * SCREEN_LENGTH, y);
  OrbitOledSetDrawMode(modOledXor);
  for(int i = 0; i<divisions; i++){
    int x = i / (double)divisions * SCREEN_LENGTH;
    OrbitOledMoveTo(x,y);
    OrbitOledDrawRect(x+1, y);
  }
  OrbitOledSetDrawMode(modOledSet);
}

void oled_paint_top_progress_bar (double current, double max)
{
  oled_paint_top_progress_bar(current,max,0);
}

void oled_paint_line_selection (int current_line)
{
  int selection_height = 6;
  int selection_width = 1;
  switch(current_line)
  {
    case 1:
      OrbitOledMoveTo(0,LINE_1_Y);
      OrbitOledDrawRect(selection_width, LINE_1_Y + selection_height);
      break;
    case 2:
      OrbitOledMoveTo(0,LINE_2_Y);
      OrbitOledDrawRect(selection_width, LINE_2_Y + selection_height);
      break;
    case 3:
      OrbitOledMoveTo(0,LINE_3_Y); OrbitOledDrawRect(selection_width, LINE_3_Y + selection_height);
      break;
  }
}

void paginate_view_string (char * input)
{
  int curr_page = 0;
  int length = strlen(input);
  int max_page = length / CHARS_PER_LINE / PAGE_LINE_SHIFT;
  int input_time = millis();
  print_string_page(input, curr_page);
  double line_prog = curr_page / max_page;
  int init_exit_switch = read_switch(0);
  while(true)
  {
    if(read_switch(0) != init_exit_switch){
      return;
    }
    if(read_button(1))
    {
      if(millis() - input_time > INPUT_TIME_THRESH)
      {
        curr_page = min(curr_page + 1, max_page);
        input_time = millis();
        print_string_page(input, curr_page);
        line_prog = curr_page / (double)max_page;
        OrbitOledMoveTo(SCREEN_LENGTH- 1,0);
        OrbitOledDrawRect(SCREEN_LENGTH - 1,line_prog * SCREEN_HEIGHT);
        OrbitOledUpdate ();
      }
    }
    if(read_button(0))
    {
      if(millis() - input_time > INPUT_TIME_THRESH)
      {
        curr_page = max(curr_page - 1, 0);
        input_time = millis();
        print_string_page(input, curr_page);
        line_prog = curr_page / (double)max_page;
        OrbitOledMoveTo(SCREEN_LENGTH- 1,0);
        OrbitOledDrawRect(SCREEN_LENGTH- 1,line_prog * SCREEN_HEIGHT);
        OrbitOledUpdate ();
      }
    }
  }
}


char * get_user_input ()
{
  orbit_moveto_line (2);
  int curr_selection = 0;
  int max_selection = 28;

  memset(user_input_buffer, 0, sizeof(user_input_buffer)/sizeof(user_input_buffer[0]));
  user_input_buffer[max_user_input_buffer - 1] = '\0';

  int input_index = 0;
  long last_insert_time = millis();

  int init_break_toggle = read_switch(0);

  while(true){
    bool isUpper = read_switch(1);
    OrbitOledClearBuffer();
    orbit_moveto_line (1);
    int scroll_offset = input_index - CHARS_PER_LINE + 1;
    if(scroll_offset < 0) scroll_offset = 0;
    OrbitOledDrawString(user_input_buffer + scroll_offset);

    if(!isUpper){
      orbit_moveto_line (2);
      send_line_to_buffer(selection_string,0);
      OrbitOledDrawString(line_buffer);
      orbit_moveto_line (3);
      send_line_to_buffer(selection_string,1);
      OrbitOledDrawString(line_buffer);
    } else {
      orbit_moveto_line (2);
      send_line_to_buffer(upper_selection_string,0);
      OrbitOledDrawString(line_buffer);
      orbit_moveto_line (3);
      send_line_to_buffer(upper_selection_string,1);
      OrbitOledDrawString(line_buffer);
    }

    //draw box around selected
    curr_selection = round(max_selection * read_pot_percent());
    int curr_col = curr_selection * CHAR_WIDTH;
    curr_col = fmod(curr_col , (CHAR_WIDTH * CHARS_PER_LINE));
    int curr_line = 2 + curr_selection / CHARS_PER_LINE;
    int xCord1 = curr_col;
    int yCord1 = get_line_y(curr_line);

    OrbitOledMoveTo (xCord1 - 1, yCord1 - 1);
    OrbitOledDrawRect(xCord1 + CHAR_WIDTH ,yCord1 + CHAR_HEIGHT + 1);
    OrbitOledUpdate ();
    if(read_button(1)){
      if( millis() - last_insert_time > insert_time_thresh )
      {
        if(!isUpper)
        {
          user_input_buffer[input_index] = selection_string[curr_selection];
        }
        else
        {
          user_input_buffer[input_index] = upper_selection_string[curr_selection];
        }
        input_index++;
        last_insert_time = millis();
      }
    }
    if(read_button(0)){
      if( millis() - last_insert_time > insert_time_thresh )
      {
        if(input_index > 0)
          user_input_buffer[--input_index] = ' ';
        last_insert_time = millis() - 25;
      }
    }
    if(read_switch(0) != init_break_toggle) {
      break;
    }
    delay (50);
  }
  return user_input_buffer;
}

//need to have orbit x cordinate as 0
void orbit_display_centered_string (const char * str)
{
  //easiest way is probably just to take the length and pad both ends with spaces
  int length = strlen(str);
  char output[CHARS_PER_LINE + 1];
  int spaces = CHARS_PER_LINE - length;

  for(int i = 0; i<spaces/2; i++){
    output[i] = ' ';
  }
  for(int i = 0; i< length; i++){
    output[i + spaces/2] = str[i];
  }
  for(int i = spaces/2 + length; i< + length + spaces; i++){
    output[i] = ' ';
  }
  output[CHARS_PER_LINE] = '\0';
  OrbitOledDrawString (output);
}

void marquee_text (char * input, unsigned long init_time, unsigned long init_delay)
{
  //check if two short
  int length = strlen(input);
  if(length < CHARS_PER_LINE)
  {
    //don't have to marquee
    OrbitOledDrawString(input);
    return;
  }
  int time_ms = length * MARQUEE_MS_PER_CHAR;
  //figure out what percent we are
  if(init_time + init_delay > millis())
  {
    OrbitOledMoveTo (0, orbit_y);
    OrbitOledDrawString(input);
    return;
  }


  init_time += init_delay;
  double percent_done = 100 * fmod(millis() - init_time, time_ms) / time_ms;
  int delay_percent = init_delay / time_ms;
  int right_percent = 50 - delay_percent;
  int left_percent = 50 - delay_percent;
  if(percent_done < left_percent)
  { //showing first half at 50 all characters should be off the screen
    int chars_showing = length * (percent_done) / (left_percent);
    OrbitOledMoveTo (0,orbit_y);
    OrbitOledDrawString(input + chars_showing);
    //going to need to figure out a different way.
  }
  else if(percent_done < left_percent + right_percent)
  {
    int chars_showing = length - length * (percent_done - (left_percent))  / right_percent;
    int print_location = chars_showing ;
    OrbitOledMoveTo (chars_showing * CHAR_WIDTH, orbit_y);
    OrbitOledDrawString(input);
  }
  else {
    OrbitOledMoveTo (0, orbit_y);
    OrbitOledDrawString(input);
    return;
  }
}
/**
  void marquee_text (const char * input, long init_time, long init_delay){
  marquee_text((char *)input, init_time,init_delay);
  } **/


void marquee_text_if_selected (char * input, unsigned long init_time, unsigned long init_delay, bool selected){
  if(selected){
    marquee_text(input,init_time,init_delay);
  }
  else{
    OrbitOledDrawString(input);
  }
}

void marquee_text_if_selected (char * input, unsigned long init_time, unsigned long init_delay, bool selected, int line){
  //don't draw if we are out of bounds
  if(line <= 0 || line > 3){
    return;
  }
  orbit_moveto_line (line);
  if(selected){
    marquee_text(input,init_time,init_delay);
  }
  else{
    OrbitOledDrawString(input);
  }
}

void display_user_prompt (const char * display_string)
{
  OrbitOledClearBuffer ();
  char confirmation[] = "okay (btn 1)";

  long init_time = millis();
  long delay_ms = 800;
  while(true){
    OrbitOledClearBuffer();
    orbit_moveto_line(1);
    marquee_text ((char *)display_string, init_time, delay_ms);
    orbit_moveto_line(3);
    orbit_display_centered_string (confirmation);
    if(read_button(1)){
      break;
    }
    OrbitOledUpdate ();
    delay(5);
  }
}

void display_user_prompt (const char * display_line_1, const char * display_line_2)
{
  OrbitOledClearBuffer ();
  char confirmation[] = "okay (btn 1)";

  long init_time = millis();
  long delay_ms = 800;
  while(true){
    OrbitOledClearBuffer();
    orbit_moveto_line(1);
    marquee_text ((char *)display_line_1, init_time, delay_ms);
    orbit_moveto_line(1);
    marquee_text ((char *)display_line_2, init_time, delay_ms);
    orbit_moveto_line(3);
    orbit_display_centered_string (confirmation);
    if(read_button(1)){
      break;
    }
    OrbitOledUpdate ();
    delay(5);
  }
}

void orbit_moveto_line (int line)
{
  switch(line)
  {
    case 1:
      orbit_y = LINE_1_Y;
      orbit_x = LINE_1_X;
      OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
      break;
    case 2:
      orbit_y = LINE_2_Y;
      orbit_x = LINE_2_X;
      OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
      break;
    case 3:
      orbit_y = LINE_3_Y;
      orbit_x = LINE_3_X;
      OrbitOledMoveTo (LINE_3_X,LINE_3_Y);
      break;
  }
}

int get_line_y(int line){
  switch(line)
  {
    case 1:
      return LINE_1_Y;
    case 2:
      return LINE_2_Y;
    case 3:
      return LINE_3_Y;
  }
  return 0;
}
