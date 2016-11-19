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

char test_string[] = "1,2,3,4,5,6,7,8,9, 10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60";

const int CHARS_PER_LINE = 16; 
const int CHAR_HEIGHT = 7; 
const double CHAR_WIDTH = 8.25; 
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
  int upper = min(line * CHARS_PER_LINE, length);
  
  int max_ind = 0; 
  for(int i = upper - CHARS_PER_LINE; i < upper; i++ )
  {
    if(!input[i]) break; 
    line_buffer[i - upper + CHARS_PER_LINE] = input[i];
    max_ind++; 
  }
  line_buffer[max_ind] = '\0'; 
  return line_buffer; 
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

char selection_string [] = "abcdefghijklmnopqrstuvwxyz          ";

void orbit_display_linewrap (char * input)
{
  OrbitOledDrawString(selection_string);
}


void get_user_input ()
{
  //first line used as input 
  //2nd and third for selection
  orbit_moveto_line (2); 
  int curr_selection = 0; 
  int max_selection = 24; 
  while(true){
    OrbitOledClear ();
    orbit_moveto_line (2); 
    send_line_to_buffer(selection_string,1);
    OrbitOledDrawString(line_buffer);
    orbit_moveto_line (3); 
    send_line_to_buffer(selection_string,2);
    OrbitOledDrawString(line_buffer);

    //draw box around selected
    curr_selection = round(max_selection * read_pot_percent());
    int curr_col = curr_selection * CHAR_WIDTH;
    curr_col = curr_col % SCREEN_LENGTH;
    int curr_line = 2 + curr_selection / CHARS_PER_LINE;
    int xCord1 = curr_col;
    int yCord1 = get_line_y(curr_line);

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
    OrbitOledMoveTo (xCord1 - 1, yCord1 - 1);
    OrbitOledDrawRect(xCord1 + CHAR_WIDTH + 1,yCord1 + CHAR_HEIGHT + 1);
    OrbitOledUpdate ();
    delay(50);
  }
}

void display_test ()
{
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






