#include "enums.h"
#include "UpdateLCD.h"
#include "ControlMode.h"
#include <Wire.h>
#include <DallasTemperature.h>

#define UP 16
#define DOWN 18
#define MENU 5
#define BUTTON_DEBOUNCE_DELAY   150


#define DEBUG1

// Data wire is plugged into port 2 on the Arduino
#define SENSOR_0 25
#define SENSOR_1 32
#define SENSOR_2 33

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire sensor_0(SENSOR_0);
OneWire sensor_1(SENSOR_1);
OneWire sensor_2(SENSOR_2);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sens0(&sensor_0);
DallasTemperature sens1(&sensor_1);
DallasTemperature sens2(&sensor_2);

#ifdef DEBUG
#include "Debug.h"
#endif
// arrays to hold device addresses
DeviceAddress inputSensor, outputSensor, outsideSensor;

void setup()
{
  Serial.begin(9600);
  sens0.begin();
  initLcd();
  printHomePage();
  getInitialData();

  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(MENU, INPUT);


}


void loop() {
  /*
     read sensors
     compare data
  */
  sens0.requestTemperatures();
  float tempC = sens0.getTempCByIndex(0);
  if (tempC != DEVICE_DISCONNECTED_C)
    print(12, 1, tempC);
  else
    Serial.println("Error: Could not read temperature data");
}

void upButton() {
  /*
     trigered on interupt
     if on main page, update desired temp value
     if on setup page and change mode is active, scroll thru control mode
  */
}

void menuButton() {
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

void getInitialData() {
  /*
     read EEPROM at starupt
  */
}
