
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

