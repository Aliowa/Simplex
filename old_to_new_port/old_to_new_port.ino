#define I2C_SDA 23
#define I2C_SCL 22
#define RED_KED 4
#define BUTTON_PIN_1 16
#define BUTTON_PIN_2 19
#define BLUE_LED 18

#define POT_PIN 26

#define DEBUG_EEPROM

#include "I2CEEPROM.h"
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x20, 16, 2);//scl-A5, sda-A4 SDA & SCL are pins A4 and A5 I2C
I2CEEPROM EEPROM(0x50);


uint8_t motorPin1 = 32;
uint8_t motorPin2 = 33;

uint64_t interval1 = 1000;//l�hike ventiili keeramine
uint64_t interval2 = 60000;//m��tmise oote interval ja kella minut
uint64_t interval3 = 4000;//pikk ventiili kerimine
uint64_t interval4 = 30000;//kui nuppe ei vajutata siis peamen��sse 0 v�i 4
uint64_t interval5 = 1000;//nupu pikk vajutus
uint64_t interval6 = 20000;//pika kerimise vaheaeg ja �ue tempi ning sulgudes in vahetumine
uint64_t interval7 = 59790;//kella minut
uint64_t interval8 = 1500;//aeg, mille jooksul men��s liikumise j�rgselt ei lubata v��rtusi muuta

uint64_t prev1Time = 0;//lyhike kerim
uint64_t prev2Time = 0;//aeg
uint64_t prev3Time = 0;//pikk kerimine
uint64_t prev4Time = 0;//kui nupud ei liigu siis tagasi menyysse
uint64_t prev5Time = 0;//kw ja out vaheldumine
uint64_t prev6Time = 0;//2e nupu pikk vajutus, men��sse
uint64_t prev7Time = 0;//jahe tagasi kuvamine
uint64_t prev8Time = 0;//aeg, mille jooksul men��s liikumise j�rgselt ei lubata v��rtusi muuta

uint64_t curTime;
uint8_t kollaState = LOW;
uint8_t siniState = LOW;

const uint8_t temp1Pin = 25;

uint16_t potZeroPoint;
const uint8_t zeroPointAddr = 19;

float temp_1;
float temp_2;
float temp_3;
uint8_t sett = 21;
float set = 1;
int mode = 1;
float mint = 0;
uint8_t maxt = 50;
int flow = 100;
uint8_t floi = 0;
float koef = 0.0;
uint8_t kontr = 1;
uint8_t suund = 1;
uint8_t koei = 0;
float sensor;
float otemp;
float itemp;
int buttonState = 0;
int button2State = 0;
int paus = 1;//v��rtuse muutmisel peatatakse temp check
int nupp = 0; //peale muutuja v��rtuse suurenemist kaitseb nupp 0 edasist v��rtuse suurenemist l�liti vabastamisel

OneWire s1(temp1Pin);

DallasTemperature st1(&s1);
DallasTemperature st2(&s1);
DallasTemperature st3(&s1);

DallasTemperature* sensor_1;
DallasTemperature* sensor_2;
DallasTemperature* sensor_3;

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);

  lcd.backlight();
  lcd.begin(16, 2);
  lcd.init();
  delay(5);

  
  sett = EEPROM.read(20);
  mint = EEPROM.read(21);
  maxt = EEPROM.read(22);
  floi = EEPROM.read(23);
  koei = EEPROM.read(24);
  kontr = EEPROM.read(25);
  suund = EEPROM.read(27);

  sensor_1 = &st1;
  sensor_2 = &st2;
  sensor_3 = &st3;
  
  pinMode(RED_KED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(RED_KED, LOW);
  digitalWrite(BLUE_LED, LOW);
  lcd.setCursor(0, 0);
  lcd.print("Simplex 36    ");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("                ");
  //digitalWrite(RED_KED, HIGH);
  //digitalWrite(BLUE_LED, HIGH);

#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("LAST STEP IN SETUP");
  delay(5000);
#endif
}

void temp_check() {
#ifdef DEBUG
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP CHECK IN");
  delay(5000);
#endif
  sensor_1->requestTemperatures();
  sensor_2->requestTemperatures();
  sensor_3->requestTemperatures();
#ifdef DEBUG
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SENS REQ OK");
  delay(5000);
#endif
  temp_1 = sensor_1->getTempCByIndex(0);
  temp_2 = sensor_2->getTempCByIndex(1);
  temp_3 = sensor_3->getTempCByIndex(2);
#ifdef DEBUG
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP CHECK OUT");
  delay(5000);
#endif
}
void kollaLOW() {
#ifdef DEBUG
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KOLLA LOW IN");
  delay(5000);
#endif
  digitalWrite(motorPin1, LOW);
  //digitalWrite(RED_KED, HIGH);
#ifdef DEBUG
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KOLLA LOW OUT");
  delay(5000);
#endif
}
void kollaHIGH() {
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("KOLLA HIGH");
  delay(5000);
#endif
  digitalWrite(RED_KED, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin1, HIGH);
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("KOLLA HIGH");
  delay(5000);
#endif
}

void siniLOW() {
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("SIMI LOW");
  delay(5000);
#endif
  digitalWrite(motorPin2, LOW);
  digitalWrite(BLUE_LED, HIGH);
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("SIMI LOW");
  delay(5000);
#endif
}
void siniHIGH() {
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("SIMI HIGH");
  delay(5000);
#endif
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
#ifdef DEBUG
  lcd.setCursor(0, 0);
  lcd.print("SIMI HIGH");
  delay(5000);
#endif
}
void loop()  {

#ifdef DEBUG_EEPROM
  Serial.println(sett);
#endif 
  uint64_t curTime = millis();
  if (paus == 1) {
    temp_check();
  }
  if (temp_3 > -127 && temp_3 < 84) {
    otemp = temp_3;
  }
  if (temp_1 > -127 && temp_1 < 84) {
    itemp = temp_1;
  }
  // Serial.print(otemp);
  //Serial.print("    ");
  //Serial.println(temp_3);
  //Serial.print(itemp);
  //Serial.print("    ");
  //Serial.println(temp_1);

  lcd.setCursor(0, 0);          //lcd �lemine rida
  if (mode == 1) {
    lcd.print("SETtmp");
  }
  if (mode == 2) {
    lcd.print("0 nurk ");
  }
  if (mode == 3) {
    lcd.print("MAXtmp");
  }
  if (mode == 4) {
    lcd.print(" L/H  ");
  }
  if (mode == 5) {
    lcd.print("koef ");
  }
  if (mode == 6 && kontr == 1) {
    lcd.print("Pysitemp ");
  }
  if (mode == 6 && kontr == 2) {
    lcd.print(" Graafik ");
  }
  if (mode == 7 && suund == 1) {
    lcd.setCursor(0, 0);
    lcd.print("vasakule");
  }
  if (mode == 7 && suund == 2) {
    lcd.setCursor(0, 0);
    lcd.print("paremale");
  }
  lcd.setCursor(6, 0);
  if (mode == 1) {
    lcd.print(sett);
    lcd.setCursor(8, 0);
    lcd.print((char)223);
  }
  if (mode == 2) {
    if (mint >= 0) {
      lcd.setCursor(7, 0);
    }
    if (mint < 0) {
      lcd.setCursor(6, 0);
    }
    lcd.print(mint);
    lcd.setCursor(8, 0);
    lcd.print(" ");
  }
  if (mode == 3) {
    lcd.print(maxt);
  }

  if (mode == 4 && floi * 10 + flow < 1000) {
    lcd.setCursor(5, 0);
    lcd.print(floi * 10 + flow);
  }
  else if (mode == 4 && floi * 10 + flow >= 1000) {
    lcd.setCursor(4, 0);
    lcd.print(floi * 10 + flow);
  }
  if (mode == 5 && koei < 90) {
    lcd.setCursor(5, 0);
    lcd.print((float)koei / 10 + koef, 1);
    lcd.setCursor(8, 0);
    lcd.print(" ");
  }
  else if (mode == 5 && koei >= 90) {
    lcd.setCursor(5, 0);
    lcd.print((float)koei / 10 + koef, 1);
  }


  lcd.setCursor(9, 0);
  if (curTime - prev5Time < interval6) {
    lcd.print(" ");
    lcd.print((floi * 10 + flow) * 4.19 * (itemp - temp_2) / 3600, 2);
    lcd.setCursor(14, 0);
    lcd.print("KW ");
    lcd.setCursor(0, 1);  //lcd ekraani alumine rida
    lcd.print("in");
    lcd.setCursor(2, 1);
    lcd.print(itemp);
    lcd.setCursor(6, 1);
    lcd.print((char)223);
  }
  if (curTime - prev5Time > interval6 && kontr == 2) {
    lcd.setCursor(9, 0);
    lcd.print("out");
    if (otemp < -9.9) {
      lcd.setCursor(11, 0);
    }
    if (otemp >= -9.9) {
      lcd.setCursor(12, 0);
    }
    lcd.print(otemp);
    lcd.setCursor(0, 1);  //lcd ekraani alumine rida
    lcd.print("(");
    lcd.setCursor(1, 1);
    lcd.print(set);
    lcd.setCursor(5, 1);
    lcd.print((char)223);
    lcd.setCursor(6, 1);
    lcd.print(")");
  }

  lcd.setCursor(7, 1);
  lcd.print("ba");
  lcd.setCursor(9, 1);
  lcd.print(temp_2);
  lcd.setCursor(13, 1);
  lcd.print((char)223);

  buttonState = digitalRead(BUTTON_PIN_1);                 // valitakse uus men�� rida, et parameetreid seadistada
  button2State = digitalRead(BUTTON_PIN_2);

  if ((buttonState || button2State) == LOW) {
    prev6Time = curTime;
  }
  if (buttonState == HIGH || button2State == HIGH) {
    paus = 2;
    prev7Time = curTime; //"JAHE" kuvamise katkestuse kell
  }

  if (buttonState == HIGH && button2State == HIGH && (sett == 22 || koei == 72 )) {//kui koei on 62 siis koef on 7.2
    mode ++;
    delay(400);
    nupp = 0;
  }
  if (curTime - prev4Time > interval4 * 0.5 ) { // kui nupud ei liigu siis peamen��sse tagasi
    paus = 1;
    if (kontr == 1) {
      mode = 1;
    }
    else if (kontr == 2 ) {
      mode = 5;
    }
    prev4Time = curTime;
  }
  if (mode > 7) {
    mode = 1;
  }

  if (buttonState == HIGH && button2State == LOW && mode == 1) {
    prev4Time = curTime;
    nupp = 1;
    if (curTime - prev6Time > interval5) {
      sett = sett + 5;
      delay(200);
    }
    if (sett > 84) {
      sett = 84;
    }
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 1 && nupp == 1) {
    sett++;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (sett > 84) {
      sett = 84;
    }
    EEPROM.write(20, sett);
  }
  if (buttonState == HIGH && button2State == LOW && mode == 2) {
    prev4Time = curTime;
    nupp = 1;
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 2 && nupp == 1) {
    mint++;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (mint > 1) {
      mint = 1;
    }
    EEPROM.write(21, mint);
  }

  if (buttonState == HIGH && button2State == LOW && mode == 3) {
    prev4Time = curTime;
    nupp = 1;
    if (curTime - prev6Time > interval5) {
      maxt = maxt + 5;
      delay(200);
    }
    if (maxt > 84) {
      maxt = 84;
    }
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 3 && nupp == 1) {
    maxt++;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (maxt > 84) {
      maxt = 84;
    }
    EEPROM.write(22, maxt);
  }
  if (buttonState == HIGH && button2State == LOW && mode == 4) {
    prev4Time = curTime;
    nupp = 1;
    if (curTime - prev6Time > interval5) {
      floi = floi + 5;
      delay(200);
    }
    if (floi > 140) {
      floi = 140;
    }
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 4 && nupp == 1) {
    floi++;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (floi > 140) {
      floi = 140;
    }
    EEPROM.write(23, floi);
  }
  if (buttonState == HIGH && button2State == LOW && mode == 5) {
    prev4Time = curTime;
    nupp = 1;
    if (curTime - prev6Time > interval5) {
      koei = koei + 5;
      delay(200);
    }
    if (koei > 110) {
      koei = 110;
    }
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 5 && nupp == 1) {
    koei++;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (koei > 110) { //koef on algselt 1 ja liites 1/10 koei 100 saame koefitsendi 10
      koei = 110;
    }
    EEPROM.write(24, koei);
  }
  if (buttonState == HIGH && button2State == LOW && mode == 6) {
    prev4Time = curTime;
    nupp = 1;
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 6 && nupp == 1) {
    kontr++;
    nupp = 0;
    if (kontr > 2) {
      kontr = 1;
    }
    EEPROM.write(25, kontr);
  }
  if (buttonState == HIGH && button2State == LOW && mode == 7) {
    prev4Time = curTime;
    nupp = 1;
  }
  if (curTime - prev8Time > interval8 && buttonState == LOW && mode == 7 && nupp == 1) {
    suund++;
    nupp = 0;
    if (suund > 2) {
      suund = 1;
    }
    EEPROM.write(27, suund);
  }

  if (button2State == HIGH && buttonState == LOW && mode == 1) {
    prev4Time = curTime;
    nupp = 2;
    if (curTime - prev6Time > interval5) {
      sett = sett - 5;
      delay(200);
    }
    if (sett < 10) {
      sett = 10;
    }
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 1 && nupp == 2) {
    sett--;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (sett < 10) {
      sett = 10;
    }
    EEPROM.write(20, sett);
  }
  if (button2State == HIGH && buttonState == LOW && mode == 2) {
    prev4Time = curTime;
    nupp = 2;
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 2 && nupp == 2) {
    mint--;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (mint < -1) {
      mint = -1;
    }
    EEPROM.write(21, mint);
  }
  if (button2State == HIGH && buttonState == LOW && mode == 3) {
    prev4Time = curTime;
    nupp = 2;
    if (curTime - prev6Time > interval5) {
      maxt = maxt - 5;
      delay(200);
    }
    if (maxt < 30) {
      maxt = 30;
    }
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 3 && nupp == 2) {
    maxt--;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (maxt < 30) {
      maxt = 30;
    }
    EEPROM.write(22, maxt);
  }
  if (button2State == HIGH && buttonState == LOW && mode == 4) {
    prev4Time = curTime;
    nupp = 2;
    if (curTime - prev6Time > interval5) {
      floi = floi - 5;
      delay(200);
    }
    if (floi < 10) {
      floi = 10;
    }
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 4 && nupp == 2) {
    floi--;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (floi < 10) {
      floi = 10;
    }
    EEPROM.write(23, floi);
  }
  if (button2State == HIGH && buttonState == LOW && mode == 5) {
    prev4Time = curTime;
    nupp = 2;
    if (curTime - prev6Time > interval5) {
      koei = koei - 5;
      delay(200);
    }
    if (koei < 1) {
      koei = 1;
    }
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 5 && nupp == 2) {
    koei--;// v��rtust suurendatakse vaid nupu vabastamisega
    nupp = 0;
    if (koei < 1) { //koef on algselt 1 ja liites 1/10 koei 100 saame koefitsendi 10
      koei = 1;
    }
    EEPROM.write(24, koei);
  }

  if (button2State == HIGH && buttonState == LOW && mode == 6) {
    prev4Time = curTime;
    nupp = 2;
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 6 && nupp == 2) {
    kontr--;
    nupp = 0;
    if (kontr < 1) {
      kontr = 2;
    }
    EEPROM.write(25, kontr);
  }
  if (button2State == HIGH && buttonState == LOW && mode == 7) {
    prev4Time = curTime;
    nupp = 2;
  }
  if (curTime - prev8Time > interval8 && button2State == LOW && mode == 7 && nupp == 2) {
    suund--;
    nupp = 0;
    if (suund < 1) {
      suund = 2;
    }
    EEPROM.write(27, suund);
  }

  if (suund < 2) {
    motorPin1 = 35;
    motorPin2 = 34;
  }
  if (suund > 1) {
    motorPin1 = 34;
    motorPin2 = 35;
  }
  if (curTime - prev5Time > interval4 + interval1 * 4) {
    prev5Time = curTime;
  }
  if (kontr == 1) {
    sensor = itemp;

    set = sett;

  }
  if (kontr == 2) {
    sensor = itemp;
    if (otemp > 0) {

      set = sett + ((20 - otemp) / (1 + (9 - ((float)koei / 10 + koef)) * 0.3));

    }
    if (temp_3 > -127 && temp_3 < 84) {
      otemp = temp_3;
    }
    if (temp_1 > -127 && temp_1 < 84) {
      itemp = temp_1;
    }
    if (otemp <= 0) {

      set = sett + (20 / (1 + (9 - ((float)koei / 10 + koef)) * 0.3)) + (-otemp / (1 + (9 - ((float)koei / 10 + koef + mint)) * 0.3));

    }
  }
  lcd.setCursor(14, 1); //alumise rea l�pp
  if (itemp > maxt - 2) {
    lcd.print("MX");
  }
  if (set + 3 < itemp) {
    lcd.print("CO");
  }
  else if (itemp - set > 0.5 && itemp - set < 3) {
    lcd.print("co");
  }
  if (set - itemp > 0.5 && set - itemp < 3) {
    lcd.print("wo");
  }
  else if (set - itemp > 3) {
    lcd.print("WO");
  }
  if (abs(set - itemp) <= 0.5) {
    lcd.print("OK");
  }

  if (curTime - prev4Time > interval4) {
    mode = 1;
    prev4Time = curTime;
  }

  if (abs(set - itemp) > 3) { //pikk kerimine, absoluutne soovitud temp erinevus on �le 3 kraadi
    if ((set - itemp) < -3) { // cool down pikk
      if (curTime - prev3Time < interval3) { //pikk
        kollaLOW();
        if (itemp >= 10) { //kui on k�rgem siis lubatakse
          siniHIGH();
        }
        if (itemp < 10) { //ei lubata min tempst allapoole minna
          siniLOW();
        }
      }
    }
    if (curTime - prev3Time > interval1 * 0.9 * (abs(set - itemp))) { // kui intervall saab t�is, siis pannakse jahutamine kinni
      siniLOW();
    }
    if (curTime - prev3Time > interval6) { // interval 2 on 1 min ja kiirkerimisel +-3 kraadi keritakse uuesti 20 sek m��dudes
      prev3Time = curTime;
    }
    // warm upp pikk
    if ((set - itemp) > 3) {
      if (curTime - prev3Time < interval3) {
        siniLOW();
        if (itemp <= maxt - 2) { //kui on madalam siis lubatakse t�sta
          kollaHIGH();
        }
        if (itemp > maxt - 2) { //ei lubata max tempst �lespoole minna
          kollaLOW();
        }
      }
    }
    if (curTime - prev3Time > interval1 * 0.9 * (abs(set - itemp))) {
      kollaLOW();
    }
    if (curTime - prev3Time > interval6) { // interval 2 on 1 min ja kiirkerimisel +-3 kraadi keritakse uuesti 20 sek m��dudes
      prev3Time = curTime;
    }
  }// pika kerimise lopppp

  if (((set - itemp) >= -3) && ((set - itemp) <= 3)) { //lyhike keeramine
    if ((set + 0.5) < itemp) { // cool down l�hike
      if (curTime - prev1Time < interval1 * 0.9) { //(*set-itemp)) { // 800 ms kontroll
        kollaLOW();
        if (itemp >= 10) { //kui on k�rgem lubatakse langetada
          siniHIGH();
        }
        if (itemp < 10) { //ei lubata min tempst allapoole minna
          siniLOW();
        }
      }// 800 ms kontroll
      if (curTime - prev1Time > interval1) { // jahutamise keeramise ja indikatsiooni l�pp
        siniLOW();
      }
    }// cool down l�hike
    if (curTime - prev1Time > interval2) {
      prev1Time = curTime;
    }
    if ((set - 0.5) > itemp)  { //warm upp l�hik
      if (curTime - prev1Time < interval1 * 0.9 * (set - itemp)) {
        siniLOW();
        if (itemp <= maxt - 2) { //ei lubata max tempst �lespoole minna
          kollaHIGH();
        }
        if (itemp > maxt - 2) { //ei lubata max tempst �lespoolepoole minna ja sulgetakse kraani
          kollaLOW();
        }
      }
      if (curTime - prev1Time >= interval1) {
        kollaLOW();
      }
    }//warm upp l�hike
    if (curTime - prev1Time > interval2) {
      prev1Time = curTime;
    }
  }//lyhike keeramine
  if (itemp > maxt) { //ei lubata max tempst �lespoole minna ja sulgetakse ventiil
    kollaLOW();

    siniHIGH();
    delay(5000);
    siniLOW();
  }
  if (((set - itemp) >= -0.5) && ((set - itemp) <= 0.5)) {
    kollaLOW();
    siniLOW();
  }
}
