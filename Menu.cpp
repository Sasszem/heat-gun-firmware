#include "Menu.hpp"

int limit(int value, int low, int high) {
  if (value>high)
    return high;
  if (value<low)
    return low;
  return value;
}

char arrow[8] = {
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};

Menu::Menu(LiquidCrystal *display)
{
    lcd = display;
    lcd->createChar(0, arrow);
}

void Menu::init(){
  lcd->begin(16, 2);
}

void Menu::rotation(int8_t direction)
{
  if (mode==0) {
    // select line mode
    currentLine += direction;
    currentLine = limit(currentLine, 0, 2);
  }
  else {
    if (direction==1)
      values[currentLine] += increments[currentLine];
    else
      values[currentLine] -= increments[currentLine];

    values[currentLine] = limit(values[currentLine], mins[currentLine], maxes[currentLine]);
  }
}

void Menu::button() {
  mode = !mode;
}

void Menu::drawLine(int line) {
  if (line==currentLine) {
    lcd->write((uint8_t)0);
    
    if (mode==1)
      lcd->print(" ");
  }
  else
    lcd->print(" ");

  char buff[30];
  snprintf ( buff, 30, formats[line], values[line]);
  lcd->print(buff);
}

void Menu::redraw()
{
  lcd->clear();
  lcd->setCursor(0,0); drawLine(currentLine);
  if (currentLine < 2)
    lcd->setCursor(0,1); drawLine(currentLine + 1);
}

