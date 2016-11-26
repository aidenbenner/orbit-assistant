#include <cmath>
#include <cstdio>
#include "utils.h"

#define abs(x) x < 0 ? -x : x 
int string_section_to_int(const char * s, int start, int end)
{
  int result = 0; 
  int digits = end - start; 
  for(int i = start; i<end; i++)
  {

    result += (s[i] - '0') * pow(10, digits - (i-start) - 1); 
  }
  return result;
}


double limit (double lower, double upper, double c)
{
  if(c < lower) c = lower; 
  if(c > upper) c = upper;
  return c;
}

double threshold (double a, double deadzone)
{
  return abs(a) > deadzone ? a < 0 ? -deadzone : deadzone : a; 
}

