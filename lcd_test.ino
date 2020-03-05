#include <LiquidCrystal.h>

const int rs = 7, en = 6, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int s1 = 11;
int s2 = 12;
int s3 = 13;
int s4 = 14;

void setup(){
  lcd.begin(20, 4);

  //=====LCD Initialize==========//
  lcd.setCursor(0, 0);
  lcd.print("==== UIU CENTeR ====");
  lcd.setCursor(0, 1);
  lcd.print("= BRRI IWD Project =");
  lcd.setCursor(0, 3);
  lcd.print("Initialize...");
  delay (5000);
  lcd.clear();
  //=====LCD Initialize==========//
}

void loop(){

  //=======================================LCD Display==============================================================//
  lcd.setCursor(0, 0); lcd.print("S1: "); lcd.setCursor(3, 0); lcd.print(s1); lcd.setCursor(6, 0);lcd.print("cm");
  lcd.setCursor(9, 0); lcd.print("||");
  lcd.setCursor(12,0);lcd.print("S2: "); lcd.setCursor(15, 0); lcd.print(s2); lcd.setCursor(18, 0);lcd.print("cm");

  lcd.setCursor(0, 1); lcd.print("S3: "); lcd.setCursor(3, 1); lcd.print(s3); lcd.setCursor(6, 1);lcd.print("cm");
  lcd.setCursor(9, 1); lcd.print("||");
  lcd.setCursor(12, 1); lcd.print("S4: "); lcd.setCursor(15, 1); lcd.print(s4); lcd.setCursor(18, 1);lcd.print("cm");

//  lcd.setCursor(0, 2); lcd.print("S3: "); lcd.setCursor(3, 2); lcd.print(s3); lcd.setCursor(6, 2);lcd.print("cm");
  lcd.setCursor(9, 2); lcd.print("||");
//  lcd.setCursor(12, 2); lcd.print("S4: "); lcd.setCursor(15, 2); lcd.print(s4); lcd.setCursor(18, 2);lcd.print("cm");
//
//  lcd.setCursor(0, 3); lcd.print("S3: "); lcd.setCursor(3, 3); lcd.print(s3); lcd.setCursor(6, 3);lcd.print("cm");
  lcd.setCursor(9, 3); lcd.print("||");
//  lcd.setCursor(12, 3); lcd.print("S4: "); lcd.setCursor(15, 3); lcd.print(s4); lcd.setCursor(18, 3);lcd.print("cm");
  //=======================================LCD Display==============================================================//
  delay(1000);
}
