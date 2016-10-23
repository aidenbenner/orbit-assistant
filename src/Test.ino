#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
#include <OrbitBoosterPackDefs.h>
#include <delay.h>
#include <LaunchPad.h>
#include <FillPat.h>
#include <string>
void setup(){
  Serial.begin(9600);
  init_sensors();

  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);

}


void InitScreen(){
  OrbitOledInit();
  OrbitOledClear();
  OrbitOledClearBuffer();
  OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
  OrbitOledSetDrawMode(modOledSet);
}

void loop(){
  print_sensor_test();
  delay(100);
} 


