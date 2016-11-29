#include <Energia.h>
#include <Arduino.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <delay.h>
#include <LaunchPad.h>
#include <FillPat.h>
#include "sensors.h"

static const uint8_t NUM_SWITCHES = 2;
static const uint8_t NUM_LEDS = 4;
static const uint8_t NUM_BUTTONS = 2;
static uint32_t P_SWITCHES[] = { PA_6 , PA_7};  
static uint32_t P_LEDS[] = { PC_6, PC_7, PD_6, PB_5}; 
static uint32_t P_BUTTONS[] = { PE_0, PD_2 }; 
static uint32_t P_POT = PE_3;

static const int POT_MAX = 4095;
static const int POT_MIN = 0;

void ghetto_assert(bool condition);

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

double read_pot_percent ()
{
  //going to assume pot is linear 
  double out = ((double)POT_MIN + read_pot()) / (double)POT_MAX;
  if(out > 1.0) out = 1.0;
  return out;
}

void set_led (int port, int val)
{
  analogWrite(P_LEDS[NUM_LEDS - 1 - port],val);
}

//encode
void led_encode_percent (double min, double max)
{
  //255 * 4 
  double equiv = min / max * 255 * 4; 
  int full_leds = equiv / 255; 
  int next_val = (int)equiv % 255; 
  for(int i = 0; i<full_leds; i++)
    set_led(i,255);
  if(full_leds != NUM_LEDS){
    set_led(full_leds,next_val);
  }
  for(int i = full_leds + 1; i<NUM_LEDS; i++){
    set_led(i,0);
  }
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

void led_gradient (int time, long init_time)
{
  double percent = 100 * (fmod(millis() - init_time, time)) / time ;  
  if(percent < 50)
  {
    set_all_led(255 * (percent) / 50 );
  }
  else
  {
    set_all_led(255 - 255 * (percent - 50) / 50);
  }
}

void led_left_right_no_wait (int time, long init_time)
{
  if(init_time == millis()) return;

  double percent = 100 * (fmod(millis() - init_time, time)) / (double)time ;  

  double curr_led; 
  if(percent < 50)
  {
    curr_led = round(2 * percent * time / (double)NUM_LEDS);
  }
  else
  {
    curr_led = round(-2 * (50 - percent) * time / (double)NUM_LEDS);
  }

  set_led(curr_led, 255); 
  for(int i = 0; i<NUM_LEDS; i++)
  {
    if(i == curr_led) continue;
    set_led(i, 0); 
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
