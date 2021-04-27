#include "LedControl.h"
#include <avr/pgmspace.h>
const byte SPACE[5] PROGMEM = {B00000000, B00000000, B00000000, B00000000, B00000000}; // SPACE
const byte A[5] PROGMEM = {B01111110, B00010001, B00010001, B01111110, B00000000}; // A
const byte B[5] PROGMEM = {B01111111, B01001001, B01001001, B00110110, B00000000}; // B
const byte C[5] PROGMEM = {B00111110, B01000001, B01000001, B00100010, B00000000}; // C
const byte D[5] PROGMEM = {B01111111, B01000001, B01000001, B00111110, B00000000}; // D
const byte E[5] PROGMEM = {B01111111, B01001001, B01001001, B01000001, B00000000}; // E
const byte F[5] PROGMEM = {B01111111, B00001001, B00001001, B00000001, B00000000}; // F
const byte G[5] PROGMEM = {B00111110, B01000001, B01001001, B01111010, B00000000}; // G
const byte H[5] PROGMEM = {B01111111, B00001000, B00001000, B01111111, B00000000}; // H
const byte I[5] PROGMEM = {B01000001, B01111111, B01000001, B00000000, B00000000}; // I
const byte J[5] PROGMEM = {B00110000, B01000000, B01000001, B00111111, B00000000}; // J
const byte K[5] PROGMEM = {B01111111, B00001000, B00010100, B01100011, B00000000}; // K
const byte L[5] PROGMEM = {B01111111, B01000000, B01000000, B01000000, B00000000}; // L
const byte M[5] PROGMEM = {B01111111, B00000010, B00001100, B00000010, B01111111}; // M
const byte N[5] PROGMEM = {B01111111, B00000100, B00001000, B00010000, B01111111}; // N
const byte O[5] PROGMEM = {B00111110, B01000001, B01000001, B00111110, B00000000}; // O
const byte P[5] PROGMEM = {B01111111, B00001001, B00001001, B00000110, B00000000}; // P
const byte Q[5] PROGMEM = {B00111110, B01000001, B01000001, B10111110, B00000000}; // Q
const byte R[5] PROGMEM = {B01111111, B00001001, B00001001, B01110110, B00000000}; // R
const byte S[5] PROGMEM = {B01000110, B01001001, B01001001, B00110010, B00000000}; // S
const byte T[5] PROGMEM = {B00000001, B00000001, B01111111, B00000001, B00000001}; // T
const byte U[5] PROGMEM = {B00111111, B01000000, B01000000, B00111111, B00000000}; // U
const byte V[5] PROGMEM = {B00001111, B00110000, B01000000, B00110000, B00001111}; // V
const byte W[5] PROGMEM = {B00111111, B01000000, B00111000, B01000000, B00111111}; // W
const byte X[5] PROGMEM = {B01100011, B00010100, B00001000, B00010100, B01100011}; // X
const byte Y[5] PROGMEM = {B00000111, B00001000, B01110000, B00001000, B00000111}; // Y
const byte Z[5] PROGMEM = {B01100001, B01010001, B01001001, B01000111, B00000000}; // Z
const int SEGMENT[10] PROGMEM = {63, 6, 91, 79, 102, 109, 125, 7, 127, 103}; // 7 segment display

/*
 pin D12 is connected to the DataIn
 pin D11 is connected to the CLK
 pin D10 is connected to CS
 
 pin A0 is connected to Vertical
 pin A1 is connected to Horizontal
 pin D9 is connected to the button
 */
LedControl lc = LedControl(12,11,10,2);

int latchPin = 2;
int dataPin = 3;
int clockPin = 4;

int toggle, dir;
int array[16][8]; //column, row
int len = 1;
bool isCandy = true, hasLost = true;
int headX = 0, headY = 4;
int del = 50;
//bool isOpenWall = true; // now digitalRead(7) is used
bool isAI = false;
int candyX, candyY;
int brightness = 1;

void setup() {
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  
  /* Set the brightness to a medium values */
  lc.setIntensity(0,brightness);
  lc.setIntensity(1,brightness);
  
  /* and clear the display */
  lc.clearDisplay(0);
  lc.clearDisplay(1);

  pinMode(9, INPUT);
  digitalWrite(9, HIGH); // to toggle button
  pinMode(7, INPUT);
  digitalWrite(7, HIGH); // to isOpenWall switch

  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
}

void displayBoard() {
  for(int i=0; i<16; i++) {
    for(int j=0; j<8; j++) {
      if(i<8) {
        lc.setLed(0,i,j,array[i][j]!=0);
      }
      else {
        lc.setLed(1,i-8,j,array[i][j]!=0);
      }
    }
  }
}
void negDisplayBoard() {
  for(int i=0; i<16; i++) {
    for(int j=0; j<8; j++) {
      if(i<8) {
        lc.setLed(0,i,j,array[i][j]==0);
      }
      else {
        lc.setLed(1,i-8,j,array[i][j]==0);
      }
    }
  }
}

void increment() {
  if(digitalRead(7) == LOW) { // swich is closed so walls are open //!isOpenWall) {
    if((dir==0 && headY==7) || (dir==1 && headX==15) || (dir==2 && headY==0) || (dir==3 && headX==0)) {
      hasLost = true;
    }
  }
  
  if(!hasLost) {
    for(int i=0; i<16; i++) {
      for(int j=0; j<8; j++) {
        if(array[i][j] > 0) {
          array[i][j]++;
        }
      }
    }
    
    if(dir==0) {
      headY = (headY+1)%8;
      isCandy = (array[headX][headY]==-1);
      hasLost = (array[headX][headY]>1);
      array[headX][headY] = 1;
    }
    else if(dir==1) {
      headX = (headX+1)%16;
      isCandy = (array[headX][headY]==-1);
      hasLost = (array[headX][headY]>1);
      array[headX][headY] = 1;
    }
    else if(dir==2) {
      headY = (headY+7)%8;
      isCandy = (array[headX][headY]==-1);
      hasLost = (array[headX][headY]>1);
      array[headX][headY] = 1;
    }
    else {
      headX = (headX+15)%16;
      isCandy = (array[headX][headY]==-1);
      hasLost = (array[headX][headY]>1);
      array[headX][headY] = 1;
    }
    
    if(isCandy) {
      len++;
    }
    else {
      for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++) {
          if(array[i][j] == len+1) {
            array[i][j] = 0;
          }
        }
      }
    }
  }
}

void scrollText(String s, int wait) {
  //int i=-17; // -17 to make text scroll from right with 17 col offset
  //while(true) {
  for(int i=-17; i<wait/50 - 17; i++) { // wait i time scrolling before going to animation
    for(int j=0; j<8; j++) {
      if(j+i >= 0) {
        byte testByte = getTextByte(s,(j+i)%(6*s.length()));
        lc.setRow(0,j,testByte);
      }
    }
    for(int j=0; j<8; j++) { // for some reason it didn't do j=0 to 15 & board j/8
      if(j+i+8 >= 0) {
        byte testByte = getTextByte(s,(j+i+8)%(6*s.length()));
        lc.setRow(1,j,testByte);
      }
    }
    delay(50);
    //i++;
    
    if(digitalRead(9) == LOW) {hasLost = false; isAI = false; return;}
  }
  hasLost = false; // new since method's outer for loop
  isAI = true;
}

byte getTextByte(String s, int byteNum) {
  byte z = B00000000;
  if(s[byteNum/6] == ' ') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(SPACE + byteNum%6);} }
  else if(s[byteNum/6] == 'A') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(A + byteNum%6);} }
  else if(s[byteNum/6] == 'B') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(B + byteNum%6);} }
  else if(s[byteNum/6] == 'C') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(C + byteNum%6);} }
  else if(s[byteNum/6] == 'D') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(D + byteNum%6);} }
  else if(s[byteNum/6] == 'E') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(E + byteNum%6);} }
  else if(s[byteNum/6] == 'F') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(F + byteNum%6);} }
  else if(s[byteNum/6] == 'G') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(G + byteNum%6);} }
  else if(s[byteNum/6] == 'H') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(H + byteNum%6);} }
  else if(s[byteNum/6] == 'I') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(I + byteNum%6);} }
  else if(s[byteNum/6] == 'J') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(J + byteNum%6);} }
  else if(s[byteNum/6] == 'K') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(K + byteNum%6);} }
  else if(s[byteNum/6] == 'L') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(L + byteNum%6);} }
  else if(s[byteNum/6] == 'M') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(M + byteNum%6);} }
  else if(s[byteNum/6] == 'N') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(N + byteNum%6);} }
  else if(s[byteNum/6] == 'O') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(O + byteNum%6);} }
  else if(s[byteNum/6] == 'P') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(P + byteNum%6);} }
  else if(s[byteNum/6] == 'Q') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(Q + byteNum%6);} }
  else if(s[byteNum/6] == 'R') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(R + byteNum%6);} }
  else if(s[byteNum/6] == 'S') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(S + byteNum%6);} }
  else if(s[byteNum/6] == 'T') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(T + byteNum%6);} }
  else if(s[byteNum/6] == 'U') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(U + byteNum%6);} }
  else if(s[byteNum/6] == 'V') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(V + byteNum%6);} }
  else if(s[byteNum/6] == 'W') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(W + byteNum%6);} }
  else if(s[byteNum/6] == 'X') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(X + byteNum%6);} }
  else if(s[byteNum/6] == 'Y') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(Y + byteNum%6);} }
  else if(s[byteNum/6] == 'Z') {if(byteNum%6 == 5) {return z;} else {return pgm_read_byte_near(Z + byteNum%6);} }
}

void displayScore(int score) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, pgm_read_byte_near(SEGMENT + score%10));
  shiftOut(dataPin, clockPin, MSBFIRST, pgm_read_byte_near(SEGMENT + score/10));
  digitalWrite(latchPin, HIGH);
}

void setUpBoard() {
  len = 1;
  headX = 0;
  headY = 4;
  for(int i=0; i<16; i++) {
    for(int j=0; j<8; j++) {
      array[i][j] = 0;
    }
  }
  dir=1;
  array[headX][headY]=1;
  randomSeed(analogRead(5));
  candyX = random(16);
  candyY = random(8);
  if(candyX!=0 or candyY!=4) {array[candyX][candyY] = -1;}
  isCandy = false;
}

int bestAIDir() { //add no walls and maze-solving algorithms
  int first, second, third, fourth;
  if(candyX-headX>=0 && candyY-headY>=0 && candyY-headY>=candyX-headX) {first=0; second=1; third=2; fourth=3;}
  else if(candyX-headX>=0 && candyY-headY>=0 && candyY-headY<candyX-headX) {first=1; second=0; third=2; fourth=3;}
  else if(candyX-headX>=0 && candyY-headY<0 && headY-candyY>=candyX-headX) {first=2; second=1; third=0; fourth=3;}
  else if(candyX-headX>=0 && candyY-headY<0 && headY-candyY<candyX-headX) {first=1; second=2; third=0; fourth=3;}
  else if(candyX-headX<0 && candyY-headY>=0 && candyY-headY>=headX-candyX) {first=0; second=3; third=1; fourth=2;}
  else if(candyX-headX<0 && candyY-headY>=0 && candyY-headY<headX-candyX) {first=3; second=0; third=1; fourth=2;}
  else if(candyX-headX<0 && candyY-headY<0 && headY-candyY>=headX-candyX) {first=2; second=3; third=0; fourth=1;}
  else if(candyX-headX<0 && candyY-headY<0 && headY-candyY<headX-candyX) {first=3; second=2; third=0; fourth=1;}
  
  else {first=0; second=1;} // when does this happen???
  
  Serial.println(first);
  Serial.println(second);
  Serial.println(" ");
  if(canMove(first)) {return first;}
  if(canMove(second)) {return second;}
  if(canMove(third)) {return third;}
  else {return fourth;}
}
bool canMove(int dirAI) {
  if((dirAI==0 && headY<7 && array[headX][headY+1]<1) ||
     (dirAI==1 && headX<15 && array[headX+1][headY]<1) ||
     (dirAI==2 && headY>0 && array[headX][headY-1]<1) ||
     (dirAI==3 && headX>0 && array[headX-1][headY]<1)) {return true;}
   else {return false;}
}

void loop() {
  displayScore(0);
  
  if(hasLost) {scrollText("SNAKE ", 12000);}
  
  setUpBoard();
  displayBoard();

  delay(1000);
  
  while(!hasLost) {
    displayScore(len-1);
    
    if(!isAI && digitalRead(9) == LOW) {
      delay(250);
      while(digitalRead(9) == HIGH) {}
      delay(250);
    }
    
    if(isCandy) {
      randomSeed(analogRead(5));
      candyX = random(16);
      candyY = random(8);
      while(array[candyX][candyY] != 0) {
        candyX = random(16);
        candyY = random(8);
      }
      array[candyX][candyY] = -1;
      isCandy = false;
    }
    
    del = analogRead(A2)/3; // speed determination from pot input
    if(!isAI) {
      for(int i=0; i<=del/10; i++) {
        delay(10);
        if(analogRead(A0)>824)
          {toggle=0;}
        else if(analogRead(A0)<200)
          {toggle=2;}
        else if(analogRead(A1)>824)
          {toggle=3;}
        else if(analogRead(A1)<200)
          {toggle=1;}
        else
          {toggle=-1;}
      }
      
      if((dir==0 || dir==2) && (toggle==1 || toggle==3))
        {dir=toggle;}
      else if((dir==1 || dir==3) && (toggle==0 || toggle==2))
        {dir=toggle;}
    }
    else {
      dir = bestAIDir();
      if(digitalRead(9) == LOW) {isAI = false; delay(1000-del); setUpBoard();}
      delay(del);
    }
    
    displayBoard();
    increment();
  }
  
  delay(1000);
  for(int count=0; count<6; count++) {
    delay(100);
    displayBoard();
    delay(100);
    //negDisplayBoard();
    lc.clearDisplay(0);
    lc.clearDisplay(1);
  }

  if(!isAI) {
    if(len >= 99) {scrollText("WINNER ", 12000);} //maximum of 128
    else {scrollText("LOSER ", 12000);}
  }
}

