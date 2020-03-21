#include <Wire.h>
#include <SPI.h>
#include <MD_DS3231.h>
#include <MD_MAX72xx.h>
#include <MD_Parola.h>

#include "Font_Data.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN       13  // or SCK
#define DATA_PIN      11  // or MOSI
#define CS_PIN        9  // or SS
#define A_BUTTON_PIN  6

#define BUF_SIZE  20
#define SPEED_TIME 0
#define PAUSE_TIME 0

// SPI hardware interface
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Text parameters
#define CHAR_SPACING  1 // pixels between characters

// Global variables
char timeBuf[7];
char secBuf[3];
char msgBuf[] = "OMEGALUL";
uint8_t degC[] = { 6, 3, 3, 56, 68, 68, 68 }; // Deg C
uint8_t intensity = 0;

boolean activeA = false;
uint8_t aMode = 0;

void getTime(char *timeC, bool blnk)
{
  RTC.readTime();
  sprintf(timeC, "%02d%c%02d", RTC.h, (blnk ? ':' : ' '), RTC.m);
}

void getSec(char *timeC)
{
  RTC.readTime();
  sprintf(timeC, "%02d", RTC.s);
}

void setup() {
  Serial.begin(57600);

  RTC.control(DS3231_CLOCK_HALT, DS3231_OFF);
  RTC.control(DS3231_12H, DS3231_OFF);
  RTC.control(DS3231_32KHZ_ENABLE, DS3231_OFF);

  P.begin(3);
  P.setInvert(false);
  
  P.setZone(0, 0, 3);
  P.setZone(1, 1, 3);
  P.setZone(2, 0, 0);
  P.setFont(0, numeric7Se);
  P.setFont(1, numeric7Se);
  P.setFont(2, numeric7Seg);

  P.addChar('*', degC);

  pinMode(A_BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  static uint32_t lastCheck = 0;
  static bool blinkState = true;

  P.setIntensity(intensity);

  Serial.println(activeA);
  
 

  if(!digitalRead(A_BUTTON_PIN)){
    activeA = !activeA;
    if(aMode < 1){
      aMode++;
    }
    else{
      aMode = 0;
    }
     delay(250);
  }
 

  P.displayAnimate();
  
  if (millis() - lastCheck >= 1000) {
    if(aMode == 0) {
    getTime(timeBuf, true);
    getSec(secBuf);
    P.displayZoneText(1, timeBuf, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
    P.displayZoneText(2, secBuf, PA_LEFT, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
    P.displayReset(1);
    P.displayReset(2);
    lastCheck = millis();
    }
    else if (aMode ==1){
    getTime(timeBuf, blinkState);
    P.displayZoneText(0, timeBuf, PA_CENTER, SPEED_TIME, PAUSE_TIME, PA_PRINT, PA_NO_EFFECT);
    blinkState = !blinkState;
    P.displayReset(0);
    lastCheck = millis();
    }

  }

   
}
