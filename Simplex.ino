#include "enums.h"
#include "UpdateLCD.h"
#include "ControlMode.h"
#include <Wire.h>
#include <DallasTemperature.h>

#define UP 16
#define DOWN 18
#define MENU 5

// Data wire is plugged into port 2 on the Arduino
#define SENSOR_0 25
#define SENSOR_1 32
#define SENSOR_2 33

#define MOTOR_PIN_1 35
#define MOTOR_PIN_2 34
#define MOTOR_PIN_3 39
#define MOTOR_PIN_4 36

#define LED_R_1 4
#define LED_R_2 17
#define LED_B_1 18
#define LED_B_2 21

#define POT_PIN 26

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire sensor_0(SENSOR_0);
OneWire sensor_1(SENSOR_1);
OneWire sensor_2(SENSOR_2);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sens0(&sensor_0);
DallasTemperature sens1(&sensor_1);
DallasTemperature sens2(&sensor_2);

DallasTemperature* sensors[3];
float temp[3];

uint64_t readSensTime;
uint16_t readInterval = 10000; //read sensors with 10s interval

void setup() {
  Serial.begin(115200);
      
  initLcd();
  printHomePage();
  getInitialData();

  sensors[0] = &sens0;
  sensors[1] = &sens1;
  sensors[2] = &sens2;

  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(MENU, INPUT);
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(MOTOR_PIN_3, OUTPUT);
  pinMode(MOTOR_PIN_4, OUTPUT);
  pinMode(LED_R_1, OUTPUT);
  pinMode(LED_R_2, OUTPUT);
  pinMode(LED_B_1, OUTPUT);
  pinMode(LED_B_2, OUTPUT);

  currentPage = HOME;

  for (int i = 0; i < 3; i++)
    sensors[i]->begin();

  readSensors();
  readSensTime = millis();
  print(12,1, temp[0]);
}

void loop() {
  if(millis() - readSensTime > readInterval && currentPage == HOME){
    readSensors();
    readSensTime = millis();
    print(12,1, temp[0]);
  }
  if (digitalRead(MENU) == HIGH)
    menuButton();
}

void upButton() {
  /*
     trigered on interupt
     if on main page, update desired temp value
     if on setup page and change mode is active, scroll thru control mode
  */
}

void menuButton() {
  switch (currentPage) {
    case HOME:
      currentPage = SETUP;
      printSetupPage();
      break;
    case SETUP:
      currentPage = HOME;
      printHomePage();
      readSensors();
      print(12,1, temp[0]);
      break;
    default: return;
  }
  delay(200); //kinda debounce
}

void downButton() {
  /*
     trigered on interupt
     if on main page, update desired temp value
     if on setup page and change mode is active, scroll thru control mode
  */
}

void readSensors() {
  for (int i = 0; i < 3; i++) {
    sensors[i]->requestTemperatures();
    temp[i] = sensors[i]->getTempCByIndex(0);
  }
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
