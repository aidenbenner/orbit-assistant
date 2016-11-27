#ifndef OLED_H
#define OLED_H 1

#define CHARS_PER_LINE  16

void oled_init ();
void draw_test ();
void display_title ();
void display_long_string ();

extern const int SCREEN_LENGTH;
extern const int SCREEN_HEIGHT;
extern const unsigned int INPUT_TIME_THRESH;  //ms
extern const int DELAY_MS;  //ms
extern const double CHAR_HEIGHT; 
extern const double CHAR_WIDTH; 
extern const int LINE_HEIGHT; 
extern const int LINE_TOP_OFFSET; 
extern const int LINE_1_Y; 
extern const int LINE_1_X; 
extern const int LINE_2_Y; 
extern const int LINE_2_X; 
extern const int LINE_3_Y; 
extern const int LINE_3_X; 
extern const int CENTERED_STR_Y; 
extern const int PAGE_LINE_SHIFT;


char * get_user_input ();
void print_string_page ( char * input, int page); 
void oled_paint_line_selection (int current_line);
void orbit_display_centered_string (const char * str);
void marquee_text (char * input, unsigned long init_time, unsigned long init_delay);
void marquee_text_if_selected (char * input, unsigned long init_time, unsigned long init_delay, bool selected);
void marquee_text_if_selected (char * input, unsigned long init_time, unsigned long init_delay, bool selected, int line);
void display_user_prompt (const char * display_string);
void oled_paint_progress_bar (double current, double max);
void oled_paint_top_progress_bar (double current, double max);
void oled_paint_top_progress_bar (double current, double max, int divisions);
void oled_draw_multiline_string(char * input, int current_line, int start_line);
void orbit_moveto_line (int line);
int get_line_y(int line); 

#endif //OLED_H
