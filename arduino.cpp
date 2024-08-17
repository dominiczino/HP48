#define TFT_MISO 22
#define TFT_MOSI 23
#define TFT_CLK 24
#define RD A3




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//             DISPLAY SETUP STUFF
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_DC A4 //other documentation calls this "write" or WR
#define TFT_CS A5
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, 9, TFT_MISO);
#define BLACK ILI9341_BLACK
#define BACKGROUND 0x950f




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//             KEYBOARD SETUP STUFF
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define keyboardSense A2
//TODO: Rename these variables to something that's not deceptive.
#define colA 10
#define colB 6
#define colC 5

#define rowA 11
#define rowB 12
#define rowC 13

//This array is here to convert between the nonsense row/col pin values into something human-comprehendable.
const byte keyNumLookup[8][8] = {{34, 24, 14, 44, 85, 54, 75, 65},
                          {35, 25, 15, 45, 86, 55, 76, 66},
                          {11, 00, 00, 00, 00, 00, 00, 00},
                          {33, 23, 13, 43, 84, 53, 74, 64},
                          {91, 00, 00, 92, 96, 93, 95, 94},
                          {32, 22, 12, 42, 83, 52, 73, 63},
                          {00, 00, 00, 00, 81, 00, 71, 61},
                          {31, 21, 00, 41, 82, 51, 72, 62}};




//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//             MISC SETUP STUFF
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define selfEnabler A1
#define SDA 20
#define SCL_I2C 21

#define serial_debug 1

enum Mode {
  MODE_ENTRY,
  MODE_ERROR,
  MODE_IDLE,
  MODE_PROCESSING,
  MODE_MENU
};

enum FunctionKeyDumps {
  F_UNIT
}

struct {
  string lineEntry = "";
  Mode mode = MODE_PROCESSING;
  byte[4] FunctionKeyState = {0,0,0,0};
  MyStack stack;
  
} machineState;



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//             MAIN SETUP FUNCTION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void setup() {
  //First, make sure we keep the lights on
  pinMode(selfEnabler, OUTPUT);
  digitalWrite(selfEnabler,HIGH);

  //Setup the keyboard sense pins
  pinMode(rowA, OUTPUT);
  pinMode(rowB, OUTPUT);
  pinMode(rowC, OUTPUT);
  pinMode(colA, OUTPUT);
  pinMode(colB, OUTPUT);
  pinMode(colC, OUTPUT);
  pinMode(keyboardSense, INPUT);

  //Start talking
  #ifdef serial_debug
    Serial.begin(9600);
  #endif


  //Get the display setup, blank out border regions, and display the backgound color.
  tft.begin();
  tft.setRotation(3);
  tft.fillRect(0,0,320,6,BLACK);
  tft.fillRect(294,0,26,240,BLACK);
  tft.fillRect(0,229,320,10,BLACK);
  tft.fillRect(0,6,294,223,BACKGROUND);
  tft.setTextColor(BLACK,BACKGROUND);
  tft.setTextSize(1);





//This function returns the human-readable key number. If no key is pressed, returns zero.
int pollKeyboard() {
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
      delayMicroseconds(10);
      if(digitalRead(keyboardSense)==HIGH) {
        return keyNumLookup[row][col];
      }
      if(row==2) {break;} //This is needed because row 2 only has one key in it, so we need to kill the loop at the first iteration
      col=col+1;
    }
    col=0;
    row=row+1;
  }
  row=0;
  return 0;
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//             MAIN LOOP FUNCTION
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void loop() {
  tft.setTextSize(3);
  tft.setCursor(50,50);
  tft.println(String(pollKeyboard()));
}