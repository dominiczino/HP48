#define selfEnabler A1
#define keyboardSense A2

#define colA 10
#define colB 6
#define colC 5

#define rowA 11
#define rowB 12
#define rowC 13


#define SDA 20
#define SCL_I2C 21

#define TFT_MISO 22
#define TFT_MOSI 23
#define TFT_CLK 24
#define RD A3



#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC A4 //other documentation calls this "write" or WR
#define TFT_CS A5

// Use hardware SPI and the above for CS/DC

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, 9, TFT_MISO);


const byte keyNumLookup[8][8] = {{34, 24, 14, 44, 85, 54, 75, 65},
                          {35, 25, 15, 45, 86, 55, 76, 66},
                          {11, 00, 00, 00, 00, 00, 00, 00},
                          {33, 23, 13, 43, 84, 53, 74, 64},
                          {91, 00, 00, 92, 96, 93, 95, 94},
                          {32, 22, 12, 42, 83, 52, 73, 63},
                          {00, 00, 00, 00, 81, 00, 71, 61},
                          {31, 21, 00, 41, 82, 51, 72, 62}};


void setup() {
  pinMode(selfEnabler, OUTPUT);  
  digitalWrite(selfEnabler,HIGH);

  
  pinMode(rowA, OUTPUT);
  pinMode(rowB, OUTPUT);
  pinMode(rowC, OUTPUT);
  pinMode(colA, OUTPUT);
  pinMode(colB, OUTPUT);
  pinMode(colC, OUTPUT);
  pinMode(keyboardSense, INPUT);
  Serial.begin(9600);

  //delay(1000);

  tft.begin();
  tft.setRotation(3);
  tft.fillRect(0,0,320,6,ILI9341_BLACK);
  tft.fillRect(294,0,26,240,ILI9341_BLACK);
  tft.fillRect(0,229,320,10,ILI9341_BLACK);
  tft.fillRect(0,6,294,223,0x950f);


  //delay(5000);

  // read diagnostics (optional but can help debug problems)
  //uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  //Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  //x = tft.readcommand8(ILI9341_RDMADCTL);
  //Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  //x = tft.readcommand8(ILI9341_RDPIXFMT);
  //Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  //x = tft.readcommand8(ILI9341_RDIMGFMT);
  //Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  //x = tft.readcommand8(ILI9341_RDSELFDIAG);
  //Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  //Serial.println(F("Benchmark                Time (microseconds)"));
  //delay(10);
  //Serial.print(F("Screen fill              "));
  //Serial.println(testFillScreen());
  //delay(500);

  //Serial.println(F("Done!"));
  tft.setRotation(3);
  tft.setTextColor(ILI9341_BLACK,ILI9341_WHITE);
  tft.setTextSize(1);
  
  
//  tft.fillScreen(ILI9341_BLACK);
//  for(int v =0; v<320; v++) {
//    tft.drawFastVLine(v,0,240,ILI9341_WHITE);
//    tft.setCursor(160,120);
//    tft.println(v);
//    delay(50);
//  }
//
//
//  tft.fillScreen(ILI9341_BLACK);
//  for(int h =0; h<240; h++) {
//    tft.drawFastHLine(0,h,320,ILI9341_WHITE);
//    tft.setCursor(160,120);
//    tft.println(h);
//    delay(50);
//  }

  //tft.fillScreen(0x7d0f);
  //tft.drawRect(0,6,294,223,ILI9341_WHITE); THIS IS THE BOUNDS OF THE SCREEN <><<<<<<<<<<<<<<<<<<<<<<<
}


// the loop function runs over and over again forever
void loop() {
  tft.setTextColor(ILI9341_BLACK,0x950f);
  tft.setTextSize(3);
  byte row = 0;
  byte col = 0;
  while(row<8) {
    while (col<8) {
      digitalWrite(rowA,bitRead(row,0));
      digitalWrite(rowB,bitRead(row,1));
      digitalWrite(rowC,bitRead(row,2));
      digitalWrite(colA,bitRead(col,0));
      digitalWrite(colB,bitRead(col,1));
      digitalWrite(colC,bitRead(col,2));
      delay(1);
      if(digitalRead(keyboardSense)==HIGH) {
        Serial.print("KEYBOARD PRESS DETECTED! ");
        Serial.println(keyNumLookup[row][col]);
        //tft.fillRect(0,6,294,223,0x950f);
        tft.setCursor(50,50);
        tft.println(String(keyNumLookup[row][col]));
        tft.println("test");
      }
      if(row==2) {break;}
      col=col+1;
    }
    col=0;
    row=row+1;
  }
  row=0;
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  tft.fillScreen(ILI9341_RED);
  yield();
  tft.fillScreen(ILI9341_GREEN);
  yield();
  tft.fillScreen(ILI9341_BLUE);
  yield();
  tft.fillScreen(ILI9341_BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(ILI9341_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextSize(2);
  tft.println();
  tft.setTextColor(ILI9341_GREEN);
  tft.println("I want to be a calculator when I grow up");
  return micros() - start;
}