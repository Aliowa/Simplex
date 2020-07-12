#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, A4, A5);

typedef enum {
  HOMEPAGE,
  SETUPPAGE
} page;


class UpdateLCD {
  public:
    UpdateLCD();
    void print(uint8_t, uint8_t, String);
    void print(uint8_t, uint8_t, int);
    void print(uint8_t, uint8_t, float);
    void printPage(page);
  private:
    void printHomePage();     //for stacic texts
    void printSetupPage();    //for stacic texts
};

UpdateLCD::UpdateLCD() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  printPage(HOMEPAGE);
}

void UpdateLCD::print (uint8_t column, uint8_t row, String text) {
  lcd.setCursor(column, row);
  lcd.print(text);
}

void UpdateLCD::print (uint8_t column, uint8_t row, int value) {
  lcd.setCursor(column, row);
  lcd.print(value);
}

void UpdateLCD::print (uint8_t column, uint8_t row, float value) {
  lcd.setCursor(column, row);
  lcd.print(value);
}

void UpdateLCD::printPage(page curretnPage) {
  switch (curretnPage) {
    case HOMEPAGE:
      printHomePage();
      break;
    case SETUPPAGE:
      printSetupPage();
      break;
    default: return;
  }
}

void UpdateLCD::printHomePage() {
  //print all static text for home page here
  const String ctrlMode  = "Control mode: ";
  print(0, 1, ctrlMode);
  print(ctrlMode.length(), 0, 0);
}

void UpdateLCD::printSetupPage() {
  //print all static text for setup page here
  const String chngCntrlMode = "Change mode: ";
  print(0,1,chngCntrlMode);
  print(chngCntrlMode.length(), 0, 0);
}
