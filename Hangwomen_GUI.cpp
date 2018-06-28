#include "Hangwomen_GUI.h"
Hangwomen_GUI::Hangwomen_GUI(): _joy_X(15), _joy_Y(9), _button_A(PORT_PIN(5, 1)){}
void Hangwomen_GUI::start(String wort, st7735s_drv *lcd, uGUI *gui){
    _gui = gui;
    _wort = wort;
    lcd->clearScreen(0x0);

    String temp = "";
    for(signed i = 0; i < wort.size()-1; i++)
    {
        temp += "_ ";
    }
    temp += "_";

    auto dotted = temp.c_str();

    _gui->SetForecolor(C_WHITE);
    _gui->FontSelect(&FONT_5X12);
    _gui->PutString(8, 10, dotted);

    _joy_X.adcMode(ADC::ADC_10_BIT);
    _joy_Y.adcMode(ADC::ADC_10_BIT);

    _button_A.gpioMode(GPIO::INPUT | GPIO::PULLUP);

    uint16_t offset_X = _joy_X.adcReadRaw();
    uint16_t offset_Y = _joy_Y.adcReadRaw();

    makeLetterTable();

    int actuelB = chooseLetter(offset_X, offset_Y);
}

int Hangwomen_GUI::chooseLetter(uint16_t offset_X, uint16_t offset_Y)
{
    int letterPosition_X[26]{8, 19, 31, 44, 56, 68, 79, 91, 103, 115, 13, 24, 36, 48, 60, 72, 84, 96, 108, 18, 30, 41, 54, 66, 78, 89};
    int letterPosition_Y[26];
    for(int i = 0; i < 26; i++)
    {
        if(i <= 9)
        {
            letterPosition_Y[i] = 79;
        }
        else if(i <= 18)
        {
            letterPosition_Y[i] = 95;
        }else
        {
            letterPosition_Y[i] = 111;
        }
    }

    int oldB = 0;
    uint16_t val_X = offset_X;
    uint16_t val_Y = offset_Y;

    while(_button_A.gpioRead()) {
            val_X = _joy_X.adcReadRaw();
            val_Y = _joy_Y.adcReadRaw();
            uint16_t temp_X = _joy_X.adcReadRaw();
            uint16_t temp_Y = _joy_Y.adcReadRaw();
            bool left = (val_X - offset_X) < -400;

            if(left)
            {
                displayStringWhite(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
                if(oldB == 0)
                {
                    displayStringRed(letterPosition_X[9], letterPosition_Y[9], _allLetter[9]);
                    oldB = 9;
                }
                else if(oldB == 10)
                {
                    displayStringRed(letterPosition_X[18], letterPosition_Y[18], _allLetter[18]);
                    oldB = 18;
                }
                else if(oldB == 19)
                {
                    displayStringRed(letterPosition_X[25], letterPosition_Y[25], _allLetter[25]);
                    oldB = 25;
                }
                else
                {
                    oldB -= 1;
                    displayStringRed(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
                }
                while((temp_X - offset_X) < -400){ temp_X = _joy_X.adcReadRaw();}
            }
            bool right = (val_X - offset_X) > 400;
            if(right)
            {
                displayStringWhite(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
                if(oldB == 9)
                {
                    displayStringRed(letterPosition_X[0], letterPosition_Y[0], _allLetter[0]);
                    oldB = 0;
                }
                else if(oldB == 18)
                {
                    displayStringRed(letterPosition_X[10], letterPosition_Y[10], _allLetter[10]);
                    oldB = 10;
                }
                else if(oldB == 25)
                {
                    displayStringRed(letterPosition_X[19], letterPosition_Y[19], _allLetter[19]);
                    oldB = 19;
                }
                else
                {
                    oldB += 1;
                    displayStringRed(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
                }
                while((temp_X - offset_X) > 400){ temp_X = _joy_X.adcReadRaw();}
            }
            bool top = (val_Y - offset_Y) > 400;
            if(top)
            {
              displayStringWhite(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
              if(oldB >= 10 && oldB <= 18)
              {
                 oldB -= 9;
              }
              else if(oldB >= 19 && oldB <= 25)
              {
                 oldB -= 8;

              }
              displayStringRed(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
              while((temp_Y - offset_Y) > 400){ temp_Y = _joy_Y.adcReadRaw();}
            }
            bool bottom = (val_Y - offset_Y) < -400;
            if(bottom)
            {
              displayStringWhite(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
              if(oldB == 0)
              {
                 oldB = 10;
              }else if(oldB == 10)
              {
                 oldB = 19;
              }
              else if(oldB == 18)
              {
                 oldB = 25;
              }
              else if(oldB >= 0 && oldB <= 9)
              {
                 oldB += 9;
              }
              else if(oldB >= 10 && oldB <= 18)
              {
                 oldB += 8;
              }
              displayStringRed(letterPosition_X[oldB], letterPosition_Y[oldB], _allLetter[oldB]);
              while((temp_Y - offset_Y) < -400){ temp_Y = _joy_Y.adcReadRaw();}
            }
        }
    return oldB;
}

void Hangwomen_GUI::displayStringRed(int x, int y, String letter)
{
    _gui->SetForecolor(C_RED);
    _gui->FontSelect(&FONT_5X12);
    _gui->PutString(x, y, letter);
}

void Hangwomen_GUI::displayStringWhite(int x, int y, String letter)
{
    _gui->SetForecolor(C_WHITE);
    _gui->FontSelect(&FONT_5X12);
    _gui->PutString(x, y, letter);
}


void Hangwomen_GUI::makeLetterTable()
{
    String zeile1 = "Q W E R T Z U I O P";
    displayStringWhite(8, 79, zeile1);
    String zeile2 = "A S D F G H J K L";
    displayStringWhite(13, 95, zeile2);
    String zeile3 = "Y X C V B N M";
    displayStringWhite(18, 111, zeile3);

    String a = "Q";
    displayStringRed(8, 79, a);

    _allLetter = "QWERTZUIOPASDFGHJKLYXCVBNM";
    //gui->DrawLine(127, 127,   0, 127, C_WHITE);
}
