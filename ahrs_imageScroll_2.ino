#include "TFT_eSPI.h"
#include <SPI.h>

#include "blueGreen.h"
#include "bgsmall.h"
#include "scale2.h"
#include "centerLine2.h"

TFT_eSPI tft= TFT_eSPI(); // Invoke library, pins defined in User_Setup.h

TFT_eSprite blueGreen  = TFT_eSprite(&tft);
TFT_eSprite scale      = TFT_eSprite(&tft);
TFT_eSprite centerLine = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite blueGreenSprite = TFT_eSprite(&tft);


#define TFT_GRN 0x2589 // New colour
#define TFT_BLA 0x05BD // New colour
// 22b14c -> 0x2589
// 00b7ef -> 0x05BD


// int imageW=300;
// int imageH=270;

// int screenW=240;
// int screenH=135;
// int m=imageW;

// int start=1;
// unsigned short imageS[32400]={0}; // edit this screenW * screen H


void setup() {
  pinMode(0,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  blueGreenSprite.createSprite(300,155);

  tft.init();
  tft.setRotation(1);
  tft.setPivot(120,67);

  tft.setSwapBytes(true);
  blueGreenSprite.setSwapBytes(true);
  scale.setSwapBytes(true);
  centerLine.setSwapBytes(true);
  
  background.createSprite(240,135);
  scale.createSprite(47,133);
  centerLine.createSprite(141,22);
  
}
const long interval = 75;
unsigned long currentMillis  = 0;
unsigned long previousMillis = 0;

int lastKnownAngle = 0;
int rollAngle = 0;
int lastKnownPosition = 0;
int direction = 1;
int position  = 0;

void loop() {
  // background.fillSprite(TFT_PURPLE);  // ------------------------
  // background.fillSprite(TFT_GRN);
  // background.fillSprite(TFT_RED);

  position = simulatePosition(lastKnownPosition);
  
  blueGreenSprite.fillSprite(TFT_BLACK);
  blueGreenSprite.fillRect(0,0,300,80-position,TFT_BLA);
  blueGreenSprite.fillRect(0,78-position,300,80+position,TFT_GRN);
  blueGreenSprite.fillRect(0,77-position,300,1,TFT_BLACK);


  scale.fillSprite(TFT_BLACK);
  centerLine.fillSprite(TFT_BLACK);

  currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;    
    rollAngle = simulateAngle(lastKnownAngle);
    }

  blueGreenSprite.pushRotated(&background,rollAngle);

  centerLine.pushImage(0,0,141,22,yellow_2);
  centerLine.pushToSprite(&background,50,66,TFT_BLACK);
  
  scale.pushImage(0,0,47,133,scaleFG2);
  scale.pushRotated(&background,rollAngle,TFT_BLACK);

  background.pushSprite(0,0);

}

// functions
int simulateAngle(int angle){
  if ((direction == 1) && (angle <= 29)) {
    angle++;
    if(angle==30) {
      direction = 0;
    }
   }
  if ((direction == 1) && (angle >= 270)) {
    angle++;
    if(angle>=360) {
      angle = 0;
    }
   }
  if ((direction == 0 ) && (angle <= 30)) {
    angle--;
    if(angle==0) {
      angle=360;
      direction = 0;
    }
   }
   if ((direction == 0 ) && (angle >= 330)) {
    angle--;
    if(angle==330) {
      direction = 1;
    }
   }
  lastKnownAngle = angle;
  return angle;
} 

int simulatePosition(int pos){
int y = 60;
 if (digitalRead(35) == 0) {
   if (pos <=y) {
    pos=lastKnownPosition + 1; 
   }
  //  delay(20);
 }

 if (digitalRead(0) == 0) {
   if (pos >= -y) {
    pos=lastKnownPosition - 1;
   }
  //  delay(20);
 }
 
 lastKnownPosition = pos;
 return pos;
}
