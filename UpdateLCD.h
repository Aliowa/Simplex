#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ROWS 2
#define COLUMNS 16
#define ADDRESS 0x20
#define I2C_SDA 23
#define I2C_SCL 22

uint8_t contrast;
uint64_t lastSetupAction;
bool setupEnd = true;

LiquidCrystal_I2C lcd(ADDRESS, COLUMNS, ROWS); // set the LCD address to 0x20 for a 16 chars and 2 line display

void initLcd() {
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
}

template <typename T>
void print(uint8_t column, uint8_t row, T value) {
  lcd.setCursor(column, row);
  lcd.print(value);
}

void printHomePage() {
  lcd.clear();
  const String homePageStrings[] = {"Mode:", "kW", "Set t:", "t:"};
  const uint8_t cursorColumnPos[] = {0, 14, 0, 10};
  const uint8_t cursorRowPos[] = {0, 0, 1, 1};
  for (int i = 0; i < 4; i++)
    print(cursorColumnPos[i], cursorRowPos[i], homePageStrings[i]);
}

void printSetupPage(int* stpPage) {
  lcd.clear();
  const String setupPage[3] = {"Change mode:", "LCD Contrast:" , "Firmware:"};
  print(0,0, setupPage[*stpPage]);
  setupEnd = false;
  lastSetupAction = millis();
  delay(200); // kinda debounce
}

void printSetupPageExtra(){
  
}
