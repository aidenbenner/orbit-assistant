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
const int INPUT_TIME_THRESH = 200;  //ms
const int DELAY_MS = 50;  //ms

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

void oled_init ()
{
  OrbitOledInit ();
  OrbitOledClear ();
  OrbitOledClearBuffer ();
  OrbitOledSetFillPattern (OrbitOledGetStdPattern (iptnSolid));
  OrbitOledSetDrawMode (modOledSet);
  OrbitOledSetCharUpdate(0);
}

static char line_buffer [CHARS_PER_LINE + 1] = " ";

/**
 * copies the ith line of the input into line_buffer
 * if 
 **/
static char * send_line_to_buffer (char * input, int line) 
{
  int max_ind = 0; 
  for(int i = 0; i < CHARS_PER_LINE; i++ )
  {
    if(!input[i]) break; 
    line_buffer[i] = input[i + line * CHARS_PER_LINE];
    max_ind++; 
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
  OrbitOledClearBuffer(); 
  send_line_to_buffer (input,curr_line);
  OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
  OrbitOledDrawString (line_buffer);

  send_line_to_buffer (input,curr_line + 1);
  OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
  OrbitOledDrawString (line_buffer);

  send_line_to_buffer (input,curr_line + 2);
  OrbitOledMoveTo (LINE_3_X,LINE_3_Y);
  OrbitOledDrawString (line_buffer);
  OrbitOledUpdate ();
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

const int insert_time_thresh = 200;  //ms
const int max_user_input = CHARS_PER_LINE * 3; 
char user_input[max_user_input * 3]; 

char * get_user_input ()
{
  orbit_moveto_line (2); 
  int curr_selection = 0; 
  int max_selection = 28; 

  memset(user_input, 0, sizeof(user_input)/sizeof(user_input[0]));
  user_input[max_user_input - 1] = '\0';

  int input_index = 0; 
  long last_insert_time = millis(); 

  int init_break_toggle = read_switch(0);

  while(true){
    bool isUpper = read_switch(1); 
    OrbitOledClearBuffer(); 
    orbit_moveto_line (1); 
    int scroll_offset = input_index - CHARS_PER_LINE + 1;
    if(scroll_offset < 0) scroll_offset = 0; 
    OrbitOledDrawString(user_input + scroll_offset);

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
          user_input[input_index] = selection_string[curr_selection];  
        } 
        else
        {
          user_input[input_index] = upper_selection_string[curr_selection];  
        }
        input_index++; 
        last_insert_time = millis(); 
      }  
    }
    if(read_button(0)){
      if( millis() - last_insert_time > insert_time_thresh ) 
      { 
        if(input_index > 0)
          user_input[--input_index] = ' ';  
        last_insert_time = millis() - 100; 
      }  
    }
    if(read_switch(0) != init_break_toggle) {
      break;
    }
    delay(50);
  }
  return user_input; 
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

void marquee_text (char * input, long init_time, long init_delay)
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
    OrbitOledMoveTo (0, _orbit_y); 
    OrbitOledDrawString(input);
    return;
  }


  init_time += init_delay;
  double percent_done = 100 * fmod(millis() - init_time, time_ms) / time_ms;
  int delay_percent = 10;
  int right_percent = 40;
  int left_percent = 40; 
  if(percent_done < delay_percent){
    OrbitOledMoveTo (0, _orbit_y); 
    OrbitOledDrawString(input);
    return;
  }
  else if(percent_done < left_percent + delay_percent) 
  { //showing first half at 50 all characters should be off the screen
    int chars_showing = length * (percent_done - delay_percent) / (left_percent); 
    OrbitOledMoveTo (0,_orbit_y); 
    OrbitOledDrawString(input + chars_showing);
    //going to need to figure out a different way. 
  }
  else 
  {
    int chars_showing = length - length * (percent_done - (left_percent + delay_percent))  / right_percent; 
    int print_location = chars_showing ;
    OrbitOledMoveTo (chars_showing * CHAR_WIDTH, _orbit_y); 
    OrbitOledDrawString(input);
  }
}

void marquee_text (const char * input, long init_time, long init_delay){
  marquee_text((char *)input, init_time,init_delay);
}

void display_user_prompt (const char * display_string)
{
  OrbitOledClearBuffer ();
  char confirmation[] = "okay (btn 1)"; 

  long init_time = millis(); 
  long delay_ms = 500; 
  while(true){
    OrbitOledClearBuffer(); 
    orbit_moveto_line(2);
    marquee_text (display_string, init_time, delay_ms); 
    orbit_moveto_line(3);
    orbit_display_centered_string (confirmation); 
    if(read_button(1)){
      break;
    }
    OrbitOledUpdate ();
  }
}

void orbit_moveto_line (int line)
{
  switch(line) 
  {
    case 1:
      _orbit_y = LINE_1_Y;
      _orbit_x = LINE_1_X;
      OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
      break;
    case 2:
      _orbit_y = LINE_2_Y;
      _orbit_x = LINE_2_X;
      OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
      break;
    case 3:
      _orbit_y = LINE_3_Y;
      _orbit_x = LINE_3_X;
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
