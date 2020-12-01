/**
  ******************************************************************************
  * @file      fanKontrolModulu.ino
  * @author    Emre TUNALI
  * @version   V1.0.1
  * @date      01 - Aralık - 2020
  * @Copyright Emre TUNALI - Simple fan control module for the Arduino Nano, 
  *            Pro Mini, Uno platform
  *            Copyright 2012-2020 (C) Emre TUNALI
  ******************************************************************************
  * @attention
  * V1.0.0 - Geliştirilmeye devam ediliyor. Kullanmayınız.
  * V1.0.1 - Geliştirilmeye devam ediliyor. Kullanmayınız.
  *           ( 2x16 LCD eklendi. )
  *           ( Giriş çıkış tanımları yapıldı. )
  *        - 
  ******************************************************************************
  */
  
#include <LiquidCrystal.h>
#include "src/FlexiTimer2.h"

const int pinCLT = A0;
const int pinFan1 = 5;
const int pinFan2 = 4;
const int buzzer = 12;
const int upButton = 2;
const int downButton = 3;

const int rs = 11, en = 10, d4 = 9, d5 = 8, d6 = 7, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

static byte fanHIGH = HIGH;
static byte fanLOW = LOW;
bool fanOn1;
bool fanOn2;

int cltSensorRaw=0;
int cltSensorRaw2=0;
double Temp=0;
int coolant = 0;

int warningTemp = 0;

int onTemp1 = 0;
int offTemp1 = 0;
int fanHyster1 = 0;

int onTemp2 = 0;
int offTemp2 = 0;
int fanHyster2 = 0;

void setup()
{
  Serial.begin(115200);
  
  pinMode(pinFan1, OUTPUT);
  pinMode(pinFan2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //pinMode(pinCLT, INPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);

  digitalWrite(pinFan1, fanLOW);
  digitalWrite(pinFan2, fanLOW);
  digitalWrite(buzzer, LOW);

  lcd.begin(16, 2);

  fanOn1 = false;
  fanOn2 = false;

  FlexiTimer2::set(10, readCLT);
  FlexiTimer2::start();
}

void fanControl1()
{
  offTemp1 = onTemp1 - fanHyster1;

  if (!fanOn1 && coolant >= onTemp1)
  {
    digitalWrite(pinFan1,fanHIGH);
    fanOn1 = true;
  }
  
  if (fanOn1 && coolant <= offTemp1)
  {
    digitalWrite(pinFan1, fanLOW);
    fanOn1 = false;
  }
}

void fanControl2()
{
  offTemp2 = onTemp2 - fanHyster2;

  if (!fanOn2 && coolant >= onTemp2)
  {
    digitalWrite(pinFan2,fanHIGH);
    fanOn2 = true;
  }
  
  if (fanOn2 && coolant <= offTemp2)
  {
    digitalWrite(pinFan2, fanLOW);
    fanOn2 = false;
  }
}

void readCLT()
{
  cltSensorRaw = analogRead(pinCLT);
  cltSensorRaw2= map(cltSensorRaw,0,1023,1023,0);

  Temp = log(((10240000/cltSensorRaw2) - 10000));
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  coolant = Temp - 273.15;
}

void loop()
{
  fanControl1();
  fanControl2();

  lcd.setCursor(0, 0);
  lcd.print(coolant);
  
  
}
