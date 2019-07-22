#include "Menu.hpp"
#include <assert.h>
namespace Menu {
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
  
  Menu::Menu(LiquidCrystal *display, Entry *ent, int num)
  {
      lcd = display;
      entries = ent;
      numEntries = num;
  }
  
  void Menu::init(){
    lcd->begin(16, 2);
    lcd->createChar(0, arrow);
  }
  
  void Menu::rotation(int8_t direction)
  {
    assert(direction==1 || direction==-1);
    if (mode==0) {
      // select line mode
      currentLine += direction;
      currentLine = limit(currentLine, 0, numEntries-1);
    }
    else {
      Entry *current = &entries[currentLine];
      
      if (direction==1)
        current->value += current->increment;
      else
        current->value -= current->increment;
  
      current->value = limit(current->value, current->minimum, current->maximum);
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
    memset(buff, 0, 30);
    snprintf ( buff, 30, entries[line].format, entries[line].value);
    lcd->print(buff);
  }
  
  void Menu::redraw()
  {
    lcd->clear();
    lcd->setCursor(0,0); drawLine(currentLine);
    if (currentLine < numEntries-1) {
      lcd->setCursor(0,1); drawLine(currentLine + 1);
    }
  }

  void Menu::setValue(int idx, int val)
  {
    assert(idx>=0&&idx<numEntries);
    entries[idx].value = val;
  }

  int Menu::getValue(int idx)
  {
    assert(idx>=0&&idx<numEntries);
    return entries[idx].value;
  }
}
