/**
  ******************************************************************************
  * @file      fanKontrolModulu-v1.ino
  * @author    Emre TUNALI
  * @version   V1.0.0
  * @date      05 - Mart - 2019
  * @Copyright Emre TUNALI - Simple fan control module for the Arduino Nano, 
  *            Pro Mini, Uno platform
  *            Copyright 2012-2019 (C) Emre TUNALI
  ******************************************************************************
  * @attention
  * V1.0.0 - Geliştirilmeye devam ediliyor. Kullanmayınız.
  *        - 
  ******************************************************************************
  */
  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FlexiTimer2.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     4

#define ADC_FILTER(input, alpha, prior) (((long)input * (256 - alpha) + ((long)prior * alpha))) >> 8
#define ADCFILTER_CLT  180

#define CALIBRATION_TABLE_SIZE 512
#define CALIBRATION_TEMPERATURE_OFFSET 40

static byte fanHIGH = HIGH;
static byte fanLOW = LOW;
bool fanOn1;
bool fanOn2;

const int pinCLT = A0;
const int pinFan1 = 8;
const int pinFan2 = 9;
const int buzzer = 10;
const int upButton = 2;
const int downButton = 3;

int coolant = 0;
int cltADC = 0;

int warningTemp = 0;

int onTemp1 = 0;
int offTemp1 = 0;
int fanHyster1 = 0;

int onTemp2 = 0;
int offTemp2 = 0;
int fanHyster2 = 0;

unsigned int tempReading;

byte cltCalibrationTable[CALIBRATION_TABLE_SIZE];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(115200);
  pinMode(pinFan1, OUTPUT);
  pinMode(pinFan2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pinCLT, INPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);

  digitalWrite(pinFan1, fanLOW);
  digitalWrite(pinFan2, fanLOW);
  digitalWrite(buzzer, LOW);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  {
    Serial.println(F("OLED BAGLANTI HATASI"));
    for(;;);
  }
  display.clearDisplay();
  display.display();

  fanOn1 = false;
  fanOn2 = false;

  FlexiTimer2::set(100, readCLT);    //100ms
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
  tempReading = analogRead(pinCLT);
  tempReading = fastMap1023toX(analogRead(pinCLT), 511); //Get the current raw CLT value
  cltADC = ADC_FILTER(tempReading, ADCFILTER_CLT, cltADC);
  //coolant = cltCalibrationTable[cltADC] - CALIBRATION_TEMPERATURE_OFFSET;
  Serial.println(cltADC);
}

void loop()
{
  
}

int fastMap1023toX(unsigned long x, int out_max)
{
  return (x * out_max) >> 10;
}
