/*
 * Hangwomen_GUI.h
 *
 *  Created on: 08.06.2018
 *      Author: student
 */

#ifndef HANGWOMEN_GUI_H_
#define HANGWOMEN_GUI_H_
#include "adc14_msp432.h"
#include "gpio_msp432.h"
#include "String.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "font_5x12.h"

using namespace std;
class Hangwomen_GUI{

public:
Hangwomen_GUI();
void start(String wort, st7735s_drv *lcd, uGUI *gui);
void makeLetterTable();
int chooseLetter(uint16_t offset_X, uint16_t offset_Y);
void displayStringRed(int x, int y, String letter);
void displayStringWhite(int x, int y, String letter);

private:
uGUI* _gui;
String _wort;
String _allLetter;
adc14_msp432_channel _joy_X;
adc14_msp432_channel _joy_Y;
gpio_msp432_pin _button_A;
};

#endif /* HANGWOMEN_GUI_H_ */
