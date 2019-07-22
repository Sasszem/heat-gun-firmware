#include <LiquidCrystal.h>

namespace Menu
{
  struct Entry {
    char *format;
    int value;
    int minimum;
    int maximum;
    int increment;
  };
  
  
  
  class Menu
  {
      private:
          LiquidCrystal *lcd;
          int currentLine = 0;
          bool mode;
          void drawLine(int line);
          Entry *entries;
          int numEntries;
      public:
          
          Menu(LiquidCrystal *display, Entry* ent, int num);
          void init();
  
  
          void rotation(int8_t direction);
          void button();
  
          void redraw();
          
          void setValue(int idx, int value);
          int getValue(int idx);
  };
}
