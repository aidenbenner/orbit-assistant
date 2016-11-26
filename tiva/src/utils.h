#ifndef UTILS_H
#define UTILS_H

//start inclusive, end exclusive
int string_section_to_int(const char * s, int start, int end);
double limit (double lower, double upper, double c);
double threshold (double a, double deadzone);
void int_to_char_arr (char * buffer, int len, int num);

#endif
