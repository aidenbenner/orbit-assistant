#ifndef SENSORS_H
#define SENSORS_H

void init_sensors (); 

bool read_button (int button); 
bool read_switch (int swit);
int read_pot ();

//leds 
void set_led (int port, int val);
void set_all_led (int val);
void led_gradient (int time);
void led_left_right (int time, bool left);
void flash_led (int port, int del);

void print_sensor_test ();

#endif

