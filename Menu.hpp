#include <LiquidCrystal.h>
#include <Arduino.h>

namespace Menu
{
  struct Entry {
    char *format;
    int value;
    int minimum;
    int maximum;
    int increment;
  };
  
  template<int N>
  class Menu
  {
      private:
          LiquidCrystal *lcd;
          int currentLine = 0;
          bool mode;
          void drawLine(int line);
          Entry entries[N];
          bool dirty = false;
      public:
          
          Menu(LiquidCrystal *display);
          void init();
          template<int idx>
          void setEntry(char *format, int minval, int maxval, int stepval, int defval);
  
          void rotation(int8_t direction);
          void button();

          void redraw(bool force = false);

          template <int idx>
          void set(int value);

          template <int idx>
          int get();
  };


  /*********************************************
   * Actual code to avoid undefined references *
   *********************************************/

  // simple limiter function
  int limit(int value, int low, int high) {
    if (value>high)
      return high;
    if (value<low)
      return low;
    return value;
  }


  // lil arrow symbol
  uint8_t arrow[8] = {
    0b10000,
    0b11000,
    0b11100,
    0b11110,
    0b11110,
    0b11100,
    0b11000,
    0b10000
  }; 
  
  // could have extern-d the above two, but not worth it. 
  // This way this file should only be included once, 
  // but makes the entiry lib one header.


  // Now to the actual class:

  template <int N>
  Menu<N>::Menu(LiquidCrystal *display)
  {
      lcd = display;
  }

  template <int N>
  template <int idx>
  void Menu<N>::setEntry(char *format, int minval, int maxval, int stepval, int defval) {
    entries[idx].format = format;
    entries[idx].value = defval;
    entries[idx].minimum = minval;
    entries[idx].maximum = maxval;
    entries[idx].increment = stepval;
  }


  template <int N>
  void Menu<N>::init(){
    lcd->begin(16, 2);
    lcd->createChar(0, arrow);
    dirty = true;
  }

  
  template <int N>
  void Menu<N>::rotation(int8_t direction)
  {
    if (direction!=1 || direction !=-1) {
      return;
    }
    if (mode==0) {
      // select line mode
      currentLine += direction;
      currentLine = limit(currentLine, 0, N-1);
    }
    else {
      Entry *current = &entries[currentLine];
      
      if (direction==1)
        current->value += current->increment;
      else
        current->value -= current->increment;
  
      current->value = limit(current->value, current->minimum, current->maximum);
    }
    dirty = true;
    Serial.println("dirty due to rotation");
  }

  
  template <int N>
  void Menu<N>::button() {
    mode = !mode;
    dirty = true;
    Serial.println("Dirty due to button");
  }

  
  template <int N>
  void Menu<N>::drawLine(int line) {
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

  
  template <int N>
  void Menu<N>::redraw(bool force)
  {
    if (!(dirty || force)) {
      return;
    }
    dirty = false;
    Serial.println("Redrawing");
    lcd->clear();
    lcd->setCursor(0,0); drawLine(currentLine);
    if (currentLine < N-1) {
      lcd->setCursor(0,1); drawLine(currentLine + 1);
    }
  }


  template <int N>
  template <int idx>
  void Menu<N>::set(int val)
  {
    static_assert((idx>=0)&&(idx<N), "Index error in set!");
    entries[idx].value = val;
    if (idx==currentLine || idx==currentLine + 1)
    {
      dirty = true;
      Serial.println("Dirty due to set");
    }
  }


  template <int N>
  template <int idx>
  int Menu<N>::get()
  {
    static_assert((idx>=0)&&(idx<N), "Index error in get()");
    return entries[idx].value;
  }
}
