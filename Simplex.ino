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

void setup() {
  Serial.begin(9600);
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


  for (int i = 0; i < 3; i++)
    sensors[i]->begin();
}


void loop() {
  readSensors();
  Serial.println("Sens 0: " + String(temp[0]) + " Sens 1: " + String(temp[1]) + " Sens 2: " + String(temp[2]));
  delay(1000);
  Serial.println("Pot position: " + String(analogRead(POT_PIN)));
  digitalWrite(MOTOR_PIN_1, HIGH);
  digitalWrite(MOTOR_PIN_2, LOW);
  digitalWrite(MOTOR_PIN_3, HIGH);
  digitalWrite(MOTOR_PIN_4, LOW);
  Serial.println("UP PIN STATE" + String(digitalRead(UP)));
  //Serial.println("Motor pin 1: " + String(digitalRead(MOTOR_PIN_1)) + " Motor pin 2: " + String(digitalRead(MOTOR_PIN_2)) + " Motor pin 3: " + String(digitalRead(MOTOR_PIN_3)) + " Motor pin 4: " + String(digitalRead(MOTOR_PIN_4)));
  delay(2000);
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
