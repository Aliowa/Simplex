#include "enums.h"
#include "UpdateLCD.h"
#include "ControlMode.h"
#include <Wire.h>
#include <DallasTemperature.h>
#define DEBUG

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
#ifdef DEBUG
#include "Debug.h"
#endif
// arrays to hold device addresses
DeviceAddress inputSensor, outputSensor, outsideSensor;

void setup()
{
  Serial.begin(9600);
  sensors.begin();
#ifdef DEBUG
  debugSensors();
#endif
  initLcd();
  printHomePage();
  getInitialData();
}


void loop() {
  /*
     read sensors
     compare data
  */
}

void upButton() {
  /*
     trigered on interupt
     if on main page, update desired temp value
     if on setup page and change mode is active, scroll thru control mode
  */
}

void enterButton() {
  /*
     enter setup mode
     enter change control mode
     hold to save, blink
  */
}

void downButton() {
  /*
     trigered on interupt
     if on main page, update desired temp value
     if on setup page and change mode is active, scroll thru control mode
  */
}

void readSensors() {
  /*
     read sensors data,
     send reference to compare data
  */
}

void compareData(int * setTemp, int * sensorData) {
  /*
     compare data
     if required, call update 3Dvent
  */
}

void update3DVent() {
  /*
     open/close valve
  */
}

void attachInterrupts() {
  /*
     attach button interrupts
  */
}

void detachInterrupts() {
  /*
     detach button interrupts
  */
}

void getInitialData() {
  /*
     read EEPROM at starupt
  */
}
