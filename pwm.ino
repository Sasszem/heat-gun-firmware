#include "Menu.hpp"
void rotaryEncoderInterrupt();

#define ROT_SW 5
#define ROT_CLK 2
#define ROT_DATA 4
const int rs = 8, en = A5, d4 = A4, d5 = A3, d6 = A2, d7 = A1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


Menu::Entry entries[3] = {
  {
    "Temp: %d \xdf" "C",
    100,
    100,
    450,
    0
  },
  {
    "Set:  %d \xdf" "C",
    100,
    100,
    450,
    10
  },
  {
    "Fan:  %d%%",
    100,
    10,
    100,
    5
  }
};


Menu::Menu menu(&lcd, entries, 3);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ROT_CLK, INPUT);
  pinMode(ROT_DATA, INPUT_PULLUP);
  pinMode(ROT_SW, INPUT_PULLUP);
  DDRD |= 1<<3;

  //////////////////////////////////////
  // DO NOT TOUCH THE FOLLOWING CODE! //
  //////////////////////////////////////
  
  // Enable timer2 ch B (PD3) - non inverting
  // Phase correct PWM TOP=OCR2A
  // (update OCR2x at TOP)
  // divide by 8 prescaler
  TCCR2A = 0b00100001;
  TCCR2B = 0b00001010;
  // enable compare A interrupt
  TIMSK2 = 0b00000010;
  
  // freq is 10 kHz
  OCR2A = 100;
  OCR2B = 0;

  //PCICR = 1;
  //PCMSK0 = 1;

  menu.init();
  menu.redraw();

  Serial.println("Setup done");
}

volatile int value = 10;

ISR(TIMER2_COMPA_vect){
  OCR2B = value;
}


// Robust Rotary encoder reading
//
// Copyright John Main - best-microcontroller-projects.com
//
// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  static uint8_t prevNextCode;
  static uint16_t store;

  prevNextCode <<= 2;
  if (digitalRead(ROT_DATA)) prevNextCode |= 0x02;
  if (digitalRead(ROT_CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}






/*
void angle(){
  bright=analogRead(pot);
  bright=map(bright,0,1023,0,10000);
  delayMicroseconds(bright);
  digitalWrite(triac, HIGH);
  delayMicroseconds(100);
  digitalWrite(triac, LOW);
}*/

unsigned long button_cooldown = 0;
int rot_dir;
void loop() {
  if (rot_dir = read_rotary())
  {
    menu.rotation(rot_dir);
    menu.redraw();
  }
  if (digitalRead(ROT_SW)!=HIGH && button_cooldown <= millis())
  {
    button_cooldown = millis() + 450;
    menu.button();
    Serial.println("MODE SWITCH");
    Serial.println(button_cooldown);
    menu.redraw();
  }
}

