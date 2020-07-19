#include <DallasTemperature.h>
#include <OneWire.h>
#include "ControlMode.h"
#include "UpdateLCD.h"

#define ONE_WIRE_BUS 14
#define MENU_UP 1
#define MENU_DOWN 2
#define MENU_ENTER 3
#define RLED1 4
#define RLED2 5
#define BLED1 6
#define BLED2 7

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer, outsideThermometer;
UpdateLCD myLcd;
ControlMode myControl;

void setup() {
  Serial.begin(9600);
  sensors.begin();
#ifdef DEBUG
  debugSensors();
#endif
  initButtonsLeds();
  attachInterrupts();

  /*
   * read EEPROM to get prev saved parameters
   */
}

void loop() {
  //read data, analyze data according to control mode, if not call funt to update 3Dvent
  getSensorData();
  comapreData();
}

void up() {
  /*if on main page, update current control parameter (seter for max T for example)
   * if on Setup page and chang contrl active(press M again after enetering setup) switch thru modes
   * 
   */
  
}

void down() {

}

void enter() {
  if (myLcd.getCurrentPage() == HOMEPAGE)
    myLcd.printPage(SETUPPAGE);
  else
  {
    ;
  }
}

void getSensorData() {

}

void comapreData() {

}

void update3DVent(control_type type) {
  switch (type) {
    case MAXTEMP: myControl.cntrlByMaxTemp();
      break;
    case GRAPH: myControl.cntrlByGraph();
      break;
    case LH: myControl.cntrlByLh();
      break;
    case NURK: myControl.cntrlByNurk();
      break;
    case KOEF: myControl.cntrlByKoef();
      break;
    case HOUR: myControl.cntrlByHour();
      break;
    default: return;
  }
}

void initButtonsLeds() {
  pinMode(MENU_UP, INPUT);
  pinMode(MENU_DOWN, INPUT);
  pinMode(MENU_ENTER, INPUT);
  pinMode(RLED1, OUTPUT);
  pinMode(RLED2, OUTPUT);
  pinMode(BLED1, OUTPUT);
  pinMode(BLED2, OUTPUT);
}

void attachInterrupts() {
  attachInterrupt(MENU_UP, up, FALLING);
  attachInterrupt(MENU_DOWN, down, FALLING);
  attachInterrupt(MENU_ENTER, enter, FALLING);
}

void detachInterrupts() {
  detachInterrupt(MENU_UP);
  detachInterrupt(MENU_DOWN);
  detachInterrupt(MENU_ENTER);
}


//=======================================DEBUG SECTION====================================

void debugSensors() {
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode())
    Serial.println("ON");
  else
    Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0))
    Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(outsideThermometer, 1))
    Serial.println("Unable to find address for Device 1");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(outsideThermometer);
  Serial.println();
}

void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
