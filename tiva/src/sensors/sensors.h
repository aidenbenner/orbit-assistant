#ifndef SENSORS_H
#define SENSORS_H

// INIT 
// this needs to be run before sensors can be read from
void init_sensors (); 

// BUTTON + SWITCHES 
bool read_button (int button); 
bool read_switch (int swit);

// POT 

// return percent of pot turned
double read_pot_percent ();
// returns analog read of pot
int read_pot ();

// LEDS
void set_led (int port, int val);
void set_all_led (int val);

// led patterns
void led_gradient (int time);
void led_gradient (int time, long init_time);

void led_left_right (int time, bool left);
void flash_led (int port, int del);

//DEBUG
void print_sensor_test ();

#endif

