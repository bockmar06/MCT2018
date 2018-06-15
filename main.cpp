
#include "gpio_msp432.h"
#include "timer_msp432.h"
#include "spi_msp432.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_5x8.h"
#include "font_5x12.h"
#include "font_6x8.h"
#include "font_6x10.h"
#include "font_7x12.h"
#include "font_8x12.h"

#include <cstdlib>

int taktrate=500000;
gpio_msp432_pin taktsender( PORT_PIN(9,0) );
gpio_msp432_pin sender( PORT_PIN(8,4) );
gpio_msp432_pin empfaenger( PORT_PIN(7,4) );

void callback1(void * arg) {
    // Convert the argument to a gpio pointer
    gpio_msp432_pin * gpio = static_cast<gpio_msp432_pin *>(arg);
    // toggle the LED
    gpio->gpioToggle();

}

void startTakt(timer_msp432 * timer1) {

    timer1->setPeriod(taktrate, TIMER::PERIODIC);
    timer1->setCallback(callback1, &taktsender);
    timer1->start();
}

/**
 * wartet bis Gegner auf 1 steht.
 */
void wartenAuf(gpio_msp432_pin * Gegner){
    while(!Gegner->gpioRead()){}
}

void WriteBool(bool wert){
    bool old = taktsender.gpioRead();
    while(old == taktsender.gpioRead()){}
    sender.gpioWrite(wert);
}

void sendeZahl(int zahl){
    WriteBool(true);
    int teiler=16;
    while(zahl!=0){
       bool wert= (zahl-teiler)>=0;
       WriteBool(wert);
       if(wert)
           zahl=zahl-teiler;
       teiler=teiler/2;
    }
    WriteBool(false);


}
int main(void)
{

    //leds
    gpio_msp432_pin led1( PORT_PIN(1,0) ); // Left red LED
    gpio_msp432_pin led2( PORT_PIN(2,0) ); // red RGB LED
    led1.gpioMode( GPIO::OUTPUT );
    led2.gpioMode( GPIO::OUTPUT );

    //sender + empfaenger
    taktsender.gpioMode(GPIO::OUTPUT);
    sender.gpioMode(GPIO::OUTPUT);
    empfaenger.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    sender.gpioWrite(false);
    empfaenger.gpioWrite(false);

    //timer & takt
    timer_msp432 timer1;
    startTakt(&timer1);

    //sende Zahl
    sendeZahl(31);

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

    while(true){

    }
}
