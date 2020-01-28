#include "Menu.hpp"

#define ROT_SW 4
#define ROT_CLK 5
#define ROT_DATA 6

#define LCD_RS A0
#define LCD_EN A1
#define LCD_D4 A2
#define LCD_D5 A3
#define LCD_D6 A4
#define LCD_D7 A5

#define TMP_CS 8
#define TMP_SO LCD_D4
#define TMP_SCK LCD_D5

#define TRIAC 9

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);


Menu::Entry entries[4] = {
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
  },
  {
    "Delay: %d us",
    10000,
    0,
    10000,
    100
  }
};


Menu::Menu menu(&lcd, entries, 4);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ROT_CLK, INPUT_PULLUP);
  pinMode(ROT_DATA, INPUT_PULLUP);
  pinMode(ROT_SW, INPUT_PULLUP);

  //////////////////////////////////////
  // DO NOT TOUCH THE FOLLOWING CODE! //
  //////////////////////////////////////
  
  // Enable timer2 ch B (PD3) - non inverting
  // Phase correct PWM TOP=OCR2A
  // (update OCR2x at TOP)
  // divide by 8 prescaler
  TCCR2A = 0b00100001;
  TCCR2B = 0b00001010;
  DDRD |= 1<<3;
  // enable compare A interrupt
  TIMSK2 = 0b00000010;
  // freq is 10 kHz
  OCR2A = 100;
  OCR2B = 0;

  // attach ZCD interrupt
  attachInterrupt(0, on_zcd, RISING);
  pinMode(TRIAC, OUTPUT);

  pinMode(TMP_CS, OUTPUT);
  digitalWrite(TMP_CS, HIGH);
  menu.init();
  menu.redraw();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Setup done");
  Serial.println("Setup done");
  
}

volatile unsigned long fire;
volatile unsigned int phase_shift;
void fire_triac() {
  unsigned long t = micros() - fire;
  if (t>=phase_shift && fire!=0 && t<15000) {
    fire = 0;
    digitalWrite(TRIAC, HIGH);
    delayMicroseconds(100);
    digitalWrite(TRIAC, LOW);
  }
}

void on_zcd() {
  cli();
  fire = micros();
  sei();
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
  fire_triac();
  if (rot_dir = read_rotary())
  {
    Serial.println("ROTATION: ");
    Serial.println(rot_dir);
    menu.rotation(rot_dir);
    menu.redraw();
    value = entries[2].value;
    phase_shift = entries[3].value;
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
