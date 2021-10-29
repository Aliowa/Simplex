/*
   Set pot to around 2047 using following:
  void setup() {
    Serial.begin(115200);
  }
  void loop() {
    Serial.println("Pot value: "+ String(analogRead(26)));
    delay(100);
  }

  Then programm unit once and comment out #define FORCE_ZERO_POINT*
*/

//i2c required pins
#define I2C_SDA 23
#define I2C_SCL 22

//LED predefine pins
#define LED_R_A 4
#define LED_R_B 17
#define LED_B_A 18
#define LED_B_B 21

//Button predefined pins
#define BUTTON_UP 16
#define BUTTON_DOWN 19
#define BUTTON_MENU 5

//Potentiometer predefined pin
#define POT_PIN 26

//Motor pins
#define MOTOR_PIN_A 32
#define MOTOR_PIN_B 33
#define MOTOR_CW digitalWrite(MOTOR_PIN_A, HIGH); digitalWrite(MOTOR_PIN_B, LOW);
#define MOTOR_CCW digitalWrite(MOTOR_PIN_A, LOW); digitalWrite(MOTOR_PIN_B, HIGH);
#define MOTOR_STOP digitalWrite(MOTOR_PIN_A, LOW); digitalWrite(MOTOR_PIN_B, LOW);

//Temeperature pins
#define TEMP_PIN 25

#include "I2CEEPROM.h"
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "LiquidCrystal_I2C.h"

OneWire one_wire_pin(TEMP_PIN);
DallasTemperature sensA(&one_wire_pin);
DallasTemperature sensB(&one_wire_pin);
DallasTemperature sensC(&one_wire_pin);
DallasTemperature* sensors[3];
float temp[3];

LiquidCrystal_I2C lcd(0x20, 16, 2); // set the LCD address to 0x20 for a 16 chars and 2 line display
I2CEEPROM eep(0x50);

//Home page static prints
const struct {
  uint8_t column;
  uint8_t row;
  String data;
} home_page[] = {
  {0, 0, "Mode:"},
  {14, 0, "kW"},
  {0, 1, "Set t:"},
  {10, 1, "t:"}
};

//Setup page static prints
const struct {
  uint8_t column;
  uint8_t row;
  String data;
} setup_page[] = {
  {0, 0, "Change Mode:"},
  {0, 0, "LCD Contrast:"},
  {0, 0, "Firmware:"}
};

struct LCDPosition {
  uint8_t column;
  uint8_t row;
};

const struct LCDPosition current_temp = {12, 1};
const struct LCDPosition set_temp = {6, 1};
const struct LCDPosition kW = {11, 0};

//EEPROM map: 20 - Set temerature; 21 - Potentiometer zero point value; 22 - Potentiomter zero point value set
struct {
  const uint8_t addres;
  uint8_t data;
} memmory_map[] = {
  {20, 0},
  {21, 0},
  {22, 0}
};

const struct {
  uint8_t min;
  uint8_t max;
} zone[] = {
  {0, 63},
  {64, 127},
  {128, 191},
  {192, 255}
};

#define FORCE_ZERO_POINT

//Set temperature min ad max
const struct {
  uint8_t min = 10;
  uint8_t max = 85;
} min_max_temp;

//Sensor reading related data
uint64_t read_sensors_time;
const uint16_t read_sensors_interval = 10000;

//Pot inital value
uint16_t pot_zero_value;

//Initial setup
void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();

  sensors[0] = &sensA;
  sensors[1] = &sensB;
  sensors[2] = &sensC;

  for (int sensor = 0; sensor < 3; sensor++)
    sensors[sensor]->begin();

  pinMode(BUTTON_UP, INPUT);
  pinMode(BUTTON_DOWN, INPUT);
  pinMode(BUTTON_MENU, INPUT);
  pinMode(MOTOR_PIN_A, OUTPUT);
  pinMode(MOTOR_PIN_B, OUTPUT);
  pinMode(LED_R_A, OUTPUT);
  pinMode(LED_R_B, OUTPUT);
  pinMode(LED_B_A, OUTPUT);
  pinMode(LED_B_B, OUTPUT);

  MOTOR_STOP

  //Print Home page static data
  for (int i = 0; i < 4; i++)
    lcd_print(home_page[i].column, home_page[i].row, home_page[i].data);


  //Read and print initial data
  read_sensors();
  lcd_print(current_temp.column, current_temp.row, temp[0]);
  memmory_map[0].data = eep.read(memmory_map[0].addres);
  lcd_print(set_temp.column, set_temp.row, memmory_map[0].data);

#ifdef FORCE_ZERO_POINT
  eep.write(memmory_map[1].addres, byte(map(analogRead(POT_PIN), 0, 4095, 0, 255)));
  eep.write(memmory_map[2].addres, 1);
#endif
}

void loop() {
  //Read sensors each {read_sensors_interval}
  //TODO! If interruped by reading pins, will hang, remove IRQ before reading
  //TODO! Move button to interrupts
  if (millis() - read_sensors_time > read_sensors_interval) {
    read_sensors();
    lcd_print(current_temp.column, current_temp.row, temp[0]);
    read_sensors_time = millis();
    update_temp();
  }


  //Check if UP button is pressed
  if (digitalRead(BUTTON_UP))
    button_up();

  //Check if DOWN button is pressed
  if (digitalRead(BUTTON_DOWN))
    button_down();

  //Check if MENU button is pressed
  if (digitalRead(BUTTON_MENU))
    button_menu();

}

template <typename T>
void lcd_print(uint8_t column, uint8_t row, T value) {
  lcd.setCursor(column, row);
  lcd.print(value);
}

void read_sensors() {
  for (int i = 0; i < 3; i++) {
    sensors[i]->requestTemperatures();
    temp[i] = sensors[i]->getTempCByIndex(0);
  }
}

void update_temp() {
  float dif_temp = memmory_map[0].data - temp[0];
  if (abs(dif_temp) > 0.1) {
    if (dif_temp > 0)
      rotate_motor(1, dif_temp);  //update to 0 if roattion should be oposite
    else
      rotate_motor(0, dif_temp);  //update to 1 if roattion should be oposite
  }
}

void rotate_motor(int direction, float dif_temp) {
  uint16_t rotation_delay = abs(dif_temp) * 1000;
  rotation_delay = constrain(rotation_delay, 100, 2000); // rotation max and min time
  Serial.println(rotation_delay);
  if (direction) {
    MOTOR_CW  //rotate clockwise
    delay(100);
    MOTOR_STOP
  } else {
    MOTOR_CCW //rotate counter clockwise
    delay(100);
    MOTOR_STOP
  }
}

//TODO! Move only ++ and --, rest do in loop
void button_up() {
  memmory_map[0].data++;
  memmory_map[0].data = constrain(memmory_map[0].data, min_max_temp.min, min_max_temp.max);
  eep.write(memmory_map[0].addres, memmory_map[0].data);
  lcd_print(set_temp.column, set_temp.row, eep.read(memmory_map[0].addres));
  delay(200); //Kind of debounce
}

void button_down() {
  memmory_map[0].data--;
  memmory_map[0].data = constrain(memmory_map[0].data, min_max_temp.min, min_max_temp.max);
  eep.write(memmory_map[0].addres, memmory_map[0].data);
  lcd_print(set_temp.column, set_temp.row, eep.read(memmory_map[0].addres));
  delay(200); //Kind of debounce
}


void button_menu() {

}
