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
  * V1.0.0 - 
  *        - 
  ******************************************************************************
  */
  
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     4

const int cltSensor = A0;
const int fan1 = 8;
const int fan2 = 9;
const int buzzer = 10;
const int upButton = 2;
const int downButton = 3;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(9600);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D))
  {
    Serial.println(F("OLED BAGLANTI HATASI"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
}

void loop()
{
  
}
