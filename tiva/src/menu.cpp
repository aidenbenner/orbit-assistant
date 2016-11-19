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
#include "serial.h"

//pre declarations 
void orbit_moveto_line (int line); 
int get_line_y (int line);

extern int xchOledMax; // defined in OrbitOled.c
extern int ychOledMax; // defined in OrbitOled.c
const int SCREEN_LENGTH = 132;
const int SCREEN_HEIGHT = 32;
const int INPUT_TIME_THRESH = 200;  //ms

char fill[] = {
  0xFF,  
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF, 
  0xFF 
};

char test_string[] = "1,2,3,4,5,6,7,8,9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60 the quick brown fox jumps over the lazy dog ";

const int CHARS_PER_LINE = 16; 
const int CHAR_HEIGHT = 7; 
const double CHAR_WIDTH = 8; 
const int LINE_HEIGHT = 10; 

const int LINE_TOP_OFFSET = 3; 
const int LINE_1_Y = LINE_TOP_OFFSET; 
const int LINE_1_X = 0; 

const int LINE_2_Y = LINE_1_Y + LINE_HEIGHT; 
const int LINE_2_X = 0; 

const int LINE_3_Y = LINE_2_Y + LINE_HEIGHT; 
const int LINE_3_X = 0; 

char line_buffer [CHARS_PER_LINE + 1] = " ";
/**
 * copies the ith line of the input into line_buffer
 * if 
 **/
char * send_line_to_buffer (char * input, int line) 
{
  int length = strlen(input);
  
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

const int PAGE_LINE_SHIFT = 1;
void print_string_page ( char * input, int page){
  int length = strlen(input);
  int lines = length / CHARS_PER_LINE; 
  int max_pages = lines / PAGE_LINE_SHIFT; 

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


void paginate_view_string (char * input) {
  int curr_page = 0; 
  int length = strlen(input);
  int max_page = length / CHARS_PER_LINE / PAGE_LINE_SHIFT;
  int input_time = millis();
  print_string_page(input, curr_page); 
  double line_prog = curr_page / max_page; 
  while(true)
  {
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

void debug_display_long_string_with_pot () ;
void display_long_string () 
{
  debug_display_long_string_with_pot ();
  return; 
  char * input = test_string;
  int length = strlen(input);
  for(int i = 0; i<length/CHARS_PER_LINE - 1; i++)
  { 
    OrbitOledClear (); 
    send_line_to_buffer (input,i);
    OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
    OrbitOledDrawString (line_buffer);

    send_line_to_buffer (input,i+1);
    OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
    OrbitOledDrawString (line_buffer);

    send_line_to_buffer (input,i+2);
    OrbitOledMoveTo (LINE_3_X,LINE_3_Y);
    OrbitOledDrawString (line_buffer);
    OrbitOledUpdate ();
    delay(2000);
    //next line
  }  
}

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
}

char selection_string [] = "abcdefghijklmnopqrstuvwxyz .@       ";
char upper_selection_string [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ .@       ";

void orbit_display_linewrap (char * input)
{
  OrbitOledDrawString(selection_string);
}


const int insert_time_thresh = 200;  //ms
const int max_user_input = CHARS_PER_LINE * 3; 
char user_input[max_user_input * 3]; 
void get_user_input ()
{
  OrbitOledSetCharUpdate(0);
  //TODO : have user string scroll right 
  //first line used as input 
  //2nd and third for selection
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
      send_line_to_buffer(selection_string,1);
      OrbitOledDrawString(line_buffer);
      orbit_moveto_line (3); 
      send_line_to_buffer(selection_string,2);
      OrbitOledDrawString(line_buffer);
    } else {
      orbit_moveto_line (2); 
      send_line_to_buffer(upper_selection_string,1);
      OrbitOledDrawString(line_buffer);
      orbit_moveto_line (3); 
      send_line_to_buffer(upper_selection_string,2);
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
    /**
    Serial.print("Curr selection | ");
    Serial.print(curr_selection);
    Serial.print(" xcord | ");
    Serial.print(xCord1);
    Serial.print(" ycord | ");
    Serial.print(yCord1);
    Serial.print(" curr_col | ");
    Serial.print(curr_col);
    Serial.print(" curr_line | ");
    Serial.println(curr_line); **/
  }
}

const int CENTERED_STR_Y = 5; 
//need to have orbit x cordinate as 0 
void orbit_display_centered_string (char * str) 
{
  //easiest way is probably just to take the length and pad both ends with spaces
  int length = strlen(str); 
  char output[CHARS_PER_LINE + 1];
  int spaces = CHARS_PER_LINE - length;

  for(int i = 0; i<spaces/2; i++){
    output[i] = ' ';
    Serial.println(i);
    Serial.println(output[i]);
  }
  for(int i = 0; i< length; i++){
    output[i + spaces/2] = str[i];
    Serial.println(i);
    Serial.println(output[i]);
  }
  for(int i = spaces/2 + length; i< + length + spaces; i++){
    output[i] = ' ';
    Serial.println(i);
    Serial.println(output[i]);
  }
  output[CHARS_PER_LINE] = '\0';
  OrbitOledDrawString (output);
}


void display_user_prompt (char * display_string)
{
  OrbitOledClear ();
  Serial.println("hit");
  char confirmation[] = "okay (btn 1)"; 
  OrbitOledMoveTo(0,LINE_2_Y); 
  orbit_display_centered_string (display_string); 
  OrbitOledMoveTo(0,LINE_3_Y);
  orbit_display_centered_string (confirmation); 
  OrbitOledUpdate ();

  while(true){
    if(read_button(1)){
      break;
    }
    //don't hog the cpu
    delay(50);
  }
}

void display_test ()
{
  paginate_view_string (test_string);
  char prompt[] = "Hi..";
  display_user_prompt(prompt);
  char prompt2[] = "Enter your name";
  display_user_prompt(prompt2);
  get_user_input (); 
}

char time_buffer[] = "12:55";
char weather_buffer[] = "12 C";

void orbit_moveto_line (int line)
{
  switch(line) 
  {
    case 1:
      OrbitOledMoveTo (LINE_1_X,LINE_1_Y);
      break;
    case 2:
      OrbitOledMoveTo (LINE_2_X,LINE_2_Y);
      break;
    case 3:
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

void display_menu ()
{
  for(int i = 0; i < 30; i++)
  { 
    OrbitOledClear ();
    orbit_moveto_line(1);
    OrbitOledDrawString (time_buffer);
    orbit_moveto_line(2);
    OrbitOledDrawString (weather_buffer);
    OrbitOledUpdate ();
    delay (200);
  }  
} 






