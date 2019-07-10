#include <LiquidCrystal.h>

class Menu
{
    private:
        LiquidCrystal *lcd;
        int currentLine = 0;
        bool mode;

        
        int increments[3] = {0, 10, 5};
        char *formats[3] = {
                "Temp: %d \xdf\x43",
                "Set:  %d \xdf\x43",
                "Fan:  %d%%"
            };
        int maxes[3] = {450, 450, 100};
        int mins[3] = {100, 100, 10};
    public:
        int values[3] = {100,100,10};
        
        Menu(LiquidCrystal *display);
        void init();


        void rotation(int8_t direction);
        void button();

        void drawLine(int line);
        void redraw();
};
