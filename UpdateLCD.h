#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ROWS 2
#define COLUMNS 16
#define ADDRESS 0x27


LiquidCrystal_I2C lcd(ADDRESS, COLUMNS, ROWS); // set the LCD address to 0x27 for a 16 chars and 2 line display

void initLcd() {
  lcd.init();
  lcd.init();
  lcd.backlight();
}

template <typename T>
void print(uint8_t columns, uint8_t rows, T value) {
  lcd.setCursor(columns, rows);
  lcd.print(value);
}

void printHomePage() {
  const String homePageStrings[] = {"Mode:", "kW", "Set t:", "t:"};
  const uint8_t cursorColumnPos[] = {0, 14, 0, 11};
  const uint8_t cursorRowPos[] = {0, 0, 1, 1};
  for (int i = 0; i < 4; i++)
    print(cursorColumnPos[i], cursorRowPos[i], homePageStrings[i]);
}

void printSetupPage() {
  const String chngCntrlMode = "Change mode.";
  print(0, 0, chngCntrlMode);
}
