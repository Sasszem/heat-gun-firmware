#include "max6675.h"


#define ROT_SW 4
#define ROT_CLK 5
#define ROT_DATA 6

#define REED 7

#define LCD_RS A0
#define LCD_EN A1
#define LCD_D4 A2
#define LCD_D5 A3
#define LCD_D6 A4
#define LCD_D7 A5

#define TMP_CS 8
#define TMP_SO A3
#define TMP_SCK A2

#define TRIAC 9
#include "Menu.hpp"

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
MAX6675 thermocouple(TMP_SCK, TMP_CS, TMP_SO);
bool reed;

Menu::Menu<5> menu(&lcd);

void setupMenu() {
  menu.setEntry<0>("Temp: %d \xdf" "C", 100, 500, 0, 10);
  menu.setEntry<1>("Set:  %d \xdf" "C", 100, 500, 10, 100);
  menu.setEntry<2>("Fan:  %d%%", 50, 100, 5, 100);
  menu.setEntry<3>("Power: %d%%", 10, 100, 5, 70);
  menu.setEntry<4>("R/H state: %02d", 0, 11, 0, 10);
}

void setupTimer() {
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
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(ROT_CLK, INPUT_PULLUP);
  pinMode(ROT_DATA, INPUT_PULLUP);
  pinMode(ROT_SW, INPUT_PULLUP);

  pinMode(REED, INPUT_PULLUP);

  setupTimer();

  // attach ZCD interrupt
  attachInterrupt(0, on_zcd, RISING);
  pinMode(TRIAC, OUTPUT);

  setupMenu();
  menu.init();
  menu.redraw();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Setup done");
  Serial.println("Setup done");
  
}

bool heat;


volatile unsigned long fire;
volatile unsigned int phase_shift;
void fire_triac() {
  unsigned long t = micros() - fire;
  if (fire!=0 && t>=phase_shift && t<15000) {
    fire = 0;
    if (heat) {
      digitalWrite(TRIAC, HIGH);
      delayMicroseconds(100);
      digitalWrite(TRIAC, LOW);
    }
  }
}

void on_zcd() {
  cli();
  fire = micros();
  sei();
}


volatile int fan_speed = 50;

ISR(TIMER2_COMPA_vect){
  OCR2B = fan_speed;
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

void handleButton() {
  static unsigned long button_cooldown;  

  if (digitalRead(ROT_SW)!=HIGH && button_cooldown <= millis())
  {
    button_cooldown = millis() + 450;
    menu.button();
    Serial.println("MODE SWITCH");
    Serial.println(button_cooldown);
  }
}

// J vs K conversion
const double conversion_constant = 0.782;
// average temperature of my workspace ( based on some guesses )
// needed to correct cold-junction compensation
// but due to that, a systemic error is also in this method!
const double average_temp = 20;

void readThermo() {
  static unsigned long cooldown;

  if (cooldown<=millis())
  {
    cooldown = millis() + 500;
    double temp_K = thermocouple.readCelsius();
    //double temp_j = (temp_K - average_temp)*conversion_constant + average_temp; // disabled for now
    menu.set<0>((int)(temp_K+0.5));
    Serial.println(menu.get<0>());
  }
}


void handleRotary() {
  int rot_dir;

  if (rot_dir = read_rotary())
  {
    Serial.println("ROTATION: ");
    Serial.println(rot_dir);
    menu.rotation(rot_dir);
  }
}

void handleReed() {
  static unsigned long cooldown;
  if (cooldown<=millis()) {
    cooldown = millis() + 1000;
    
    reed = digitalRead(REED)==LOW;
    menu.set<4>((reed ? 10 : 0)+(heat ? 1 : 0));
  }
}

void shouldHeat() {
  heat = !reed && (menu.get<0>()<menu.get<1>());
}


inline int power_to_delay(int power) {
  return map(power, 0, 100, 10000, 1000);
}

void loop() {
  fire_triac();
  handleButton();
  handleRotary();
  readThermo();
  handleReed();
  shouldHeat();
  phase_shift = power_to_delay(menu.get<3>());
  fan_speed = menu.get<2>();
  menu.redraw();
}
