#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitBoosterPackDefs.h>
#include <delay.h>
#include <LaunchPad.h>
#include <FillPat.h>

static const uint8_t NUM_SWITCHES = 2;
static const uint8_t NUM_LEDS = 3;
static const uint8_t NUM_BUTTONS = 2;
static const uint32_t P_SWITCHES[] = { PA_6 , PA_7};  
static const uint32_t P_LEDS[] = { PC_6, PC_7, PD_6, PB_5}; 
static const uint32_t P_BUTTONS[] = { PE_0, PD_2 }; 
static const uint32_t P_POT = PE_3;

void init_sensors ()
{
  for(int i = 0; i < NUM_SWITCHES; i++)
    pinMode(P_SWITCHES[i], INPUT);

  for(int i = 0; i < NUM_BUTTONS; i++)
    pinMode(P_BUTTONS[i], INPUT);

  for(int i = 0; i < NUM_LEDS; i++)
    pinMode(P_LEDS[i], OUTPUT);
}

bool read_button (int button)
{
  ghetto_assert(button >= 0 && button < NUM_BUTTONS);
  return digitalRead(P_BUTTONS[button]);
}

bool read_switch (int swit)
{
  return digitalRead(P_SWITCHES[swit]);
}

int read_pot ()
{
  return analogRead(P_POT);
}

void set_led (int port, int val)
{
  analogWrite(P_LEDS[port],val);
}

void set_all_led (int val)
{
  for(int i = 0; i<NUM_LEDS; i++)
  {
    analogWrite(P_LEDS[i],val);
  }
}

void led_gradient (int time)
{
  int del = time / 255; 
  for(int i = 0; i<255; i++ )
  {
    set_all_led(i);
    delay(del);
  }
  for(int i = 255; i>=0; i--)
  {
    set_all_led(i);
    delay(del);
  }
}

void led_left_right (int time, bool left)
{
  int del = time/NUM_LEDS;
  for(int i = 0; i<NUM_LEDS; i++)
  {
    int port = left ? NUM_LEDS - 1 - i : i; 
    set_led(port , 255); 
    delay(del);
    set_led(port , 0); 
  }
}

void led_left_right (int time)
{
  led_left_right(time/2, true);
  led_left_right(time/2, false);
}

void ghetto_assert(bool condition)
{
  if(!condition)
  {

  }
}

void flash_led (int port, int del)
{
  set_led(port,255);
  delay(del);
  set_led(port,0);
  delay(del);
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

void print_sensor_test ()
{
  OrbitOledClear();
  OrbitOledMoveTo(10,10);
  char s[10];  String(read_pot()).toCharArray(s,10);
  char b1[10]; String(read_button(0)).toCharArray(b1,10);
  char b2[10]; String(read_button(1)).toCharArray(b2,10);
  char s1[10]; String(read_switch(0)).toCharArray(s1,10);
  char s2[10]; String(read_switch(1)).toCharArray(s2,10);
  OrbitOledDrawString(s);
  OrbitOledMoveTo(60,10);
  OrbitOledDrawString(b1);
  OrbitOledMoveTo(90,10);
  OrbitOledDrawString(b2);
  OrbitOledMoveTo(110,10);
  OrbitOledDrawString(s1);
  OrbitOledMoveTo(80,20);
  OrbitOledDrawString(s2);
  OrbitOledUpdate();
}
