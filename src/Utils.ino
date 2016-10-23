#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitBoosterPackDefs.h>
#include <delay.h>
#include <LaunchPad.h>
#include <FillPat.h>

const uint8_t NUM_SWITCHES = 2;
const uint8_t NUM_LEDS = 3;
const uint8_t NUM_BUTTONS = 2;
const uint32_t P_SWITCHES[] = { PA_6 , PA_7};  
const uint32_t P_LEDS[] = { PC_6, PC_7, PD_6, PB_5}; 
const uint32_t P_BUTTONS[] = { PE_0, PD_2 }; 
const uint32_t P_POT = PE_3;

void InitSensors(){
    for(int i = 0; i<NUM_SWITCHES; i++)
        pinMode(P_SWITCHES[i], INPUT);

    for(int i = 0; i<NUM_BUTTONS; i++)
        pinMode(P_BUTTONS[i], INPUT);

    for(int i = 0; i<NUM_LEDS; i++)
        pinMode(P_LEDS[i], OUTPUT);
}

bool ReadButton(int button){
    GhettoAssert(button >= 0 && button < NUM_BUTTONS);
    return digitalRead(P_BUTTONS[button]);
}

bool ReadSwitch(int swit){
    return digitalRead(P_SWITCHES[swit]);
}


int ReadPot(){
    return analogRead(P_POT);
}



void SetLed(int port, int val){
    analogWrite(P_LEDS[port],val);
}

void SetAllLed(int val){
    for(int i = 0; i<NUM_LEDS; i++){
        analogWrite(P_LEDS[i],val);
    }
}

void LedGradient(int time){
    int del = time / 255; 
    for(int i = 0; i<255; i++ ){
        SetAllLed(i);
        delay(del);
    }
    for(int i = 255; i>=0; i--){
        SetAllLed(i);
        delay(del);
    }
}

void LedLeftRight(int time, bool left){
    int del = time/NUM_LEDS;
    for(int i = 0; i<NUM_LEDS; i++){
        int port = left ? NUM_LEDS - 1 - i : i; 
        SetLed(port , 255); 
        delay(del);
        SetLed(port , 0); 
    }
}

void LedLeftRight(int time){
    LedLeftRight(time/2, true);
    LedLeftRight(time/2, false);
}

void GhettoAssert(bool condition){
    if(!condition){

    }
}

void FlashLed(int port, int del){
    SetLed(port,255);
    delay(del);
    SetLed(port,0);
    delay(del);
}

double Limit(double lower, double upper, double c){
    if(c < lower) c = lower; 
    if(c > upper) c = upper;
    return c;
}

double Threshold(double a, double deadzone){
    return abs(a) > deadzone ? a < 0 ? -deadzone : deadzone : a; 
}

void PrintSensorTest(){
    OrbitOledClear();
    OrbitOledMoveTo(10,10);
    char s[10];  String(ReadPot()).toCharArray(s,10);
    char b1[10]; String(ReadButton(0)).toCharArray(b1,10);
    char b2[10]; String(ReadButton(1)).toCharArray(b2,10);
    char s1[10]; String(ReadSwitch(0)).toCharArray(s1,10);
    char s2[10]; String(ReadSwitch(1)).toCharArray(s2,10);
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
