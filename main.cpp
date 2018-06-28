// ---------------------------------------------
//           This file is part of
//      _  _   __    _   _    __    __
//     ( \/ ) /__\  ( )_( )  /__\  (  )
//      \  / /(__)\  ) _ (  /(__)\  )(__
//      (__)(__)(__)(_) (_)(__)(__)(____)
//
//     Yet Another HW Abstraction Library
//      Copyright (C) Andreas Terstegge
//      BSD Licensed (see file LICENSE)
//
// ---------------------------------------------
//
// A simple example for displaying text, lines and
// circles on the LCD using the uGUI library.
// See the doc folder in YAHAL/src/uGUI for more
// information on the uGUI API.

#include "spi_msp432.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"
#include "String.h"
#include "Hangwomen_GUI.h"

#include <cstdlib>

int main(void)
{

    // Setup SPI interface
    gpio_msp432_pin lcd_cs (PORT_PIN(5, 0));
    spi_msp432  spi(EUSCI_B0_SPI, lcd_cs);
    spi.setSpeed(24000000);

    // Setup LCD driver
    gpio_msp432_pin lcd_rst(PORT_PIN(5, 7));
    gpio_msp432_pin lcd_dc (PORT_PIN(3, 7));
    st7735s_drv lcd(spi, lcd_rst, lcd_dc, st7735s_drv::Crystalfontz_128x128);

    // Setup uGUI
    uGUI gui(lcd);

    lcd.clearScreen(0x0);

    gui.SetForecolor(C_YELLOW);
    gui.FontSelect(&FONT_4X6);
    gui.PutString(4, 3, "Hallo MATSE");

    gui.SetForecolor(C_GREEN);
    gui.FontSelect(&FONT_5X8);
    gui.PutString(4, 9, "Hallo MATSE");

    gui.SetForecolor(C_CYAN);
    gui.FontSelect(&FONT_5X12);
    gui.PutString(4, 18, "Hallo MATSE");

    gui.SetForecolor(C_RED);
    gui.FontSelect(&FONT_6X8);
    gui.PutString(4, 31, "Hallo MATSE");

    gui.SetForecolor(C_WHITE);
    gui.FontSelect(&FONT_6X10);
    gui.PutString(4, 40, "Hallo MATSE");

    gui.SetForecolor(C_MAGENTA);
    gui.FontSelect(&FONT_7X12);
    gui.PutString(4, 51, "Hallo MATSE");

    gui.SetForecolor(C_ORANGE);
    gui.FontSelect(&FONT_8X12);
    gui.PutString(4, 64, "Hallo MATSE");

    // Draw some lines and circles
    gui.DrawCircle(30, 96, 15, C_YELLOW);
    gui.FillCircle(30, 96, 13, C_RED);

    gui.DrawLine(0,     0, 127,   0, C_YELLOW);
    gui.DrawLine(127,   0, 127, 127, C_YELLOW);
    gui.DrawLine(127, 127,   0, 127, C_YELLOW);
    gui.DrawLine(0,   127,   0,   0, C_YELLOW);

    // Draw some random-colored pixels
    for (uint16_t x=64; x < 120; x++) {
        for (uint16_t y=80; y < 120; y++) {
            gui.DrawPixel(x, y, rand() % 0x1000000);
        }
    }
    Hangwomen_GUI hangwomen;
    hangwomen.start("Wasserrate", &lcd, &gui);
}
