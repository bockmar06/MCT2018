#include "timer_msp432.h"
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
#include <time.h>
#include "Hangwomen_GUI.h


#include <cstdlib>

int taktrate=50000;
gpio_msp432_pin taktsender( PORT_PIN(4,7) );
gpio_msp432_pin sender( PORT_PIN(5,4) );
gpio_msp432_pin empfaenger( PORT_PIN(5,5) );
timer_msp432 timer1;
uGUI* uguiptr=nullptr;

int leseZahl();
void leseerror();
void callback2(void * arg);
void waithalbertakt();
void WriteBool(bool wert);
bool ReadBool();

void callback1(void * arg) {
    // Convert the argument to a gpio pointer
    gpio_msp432_pin * gpio = static_cast<gpio_msp432_pin *>(arg);
    // toggle the LED
    gpio->gpioToggle();

}

void startTakt() {

    timer1.setPeriod(taktrate, TIMER::PERIODIC);
    timer1.setCallback(callback1, &taktsender);
    timer1.start();
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
    for(int i=0;i<5;i++){
       bool wert= (zahl-teiler)>=0;
       WriteBool(wert);
       if(wert)
           zahl=zahl-teiler;
       teiler=teiler/2;
    }
    WriteBool(false);
}


bool ReadBool(void){
    bool old = taktsender.gpioRead();
    while(old == taktsender.gpioRead()){}
    waithalbertakt();
    empfaenger.gpioRead();
}
void callback2(void * arg) {
    (*static_cast<bool *>(arg)) = true;
}

void waithalbertakt() {
    // Setup the second time in ONE_SHOT mode
    // and simply wait until the callback
    // method has changed the 'expired' variable.
    // The pointer to this variable (which is
    // located on the stack!) is passed as an
    // argument to the callback method, so the
    // callback method can modify this variable
    // (see above).
    bool expired = false;
    timer_msp432 timer2(TIMER32_2);
    timer2.setPeriod(taktrate/2, TIMER::ONE_SHOT);
    timer2.setCallback(callback2, &expired);
    timer2.start();

    // Now wait until the callback method
    // has changed the 'expired' variable...
    while(!expired) ;
}


int begriff(bool ishost){
 //2 unterschiedliche Begriffsdatenbanken mit max 32 (0-31) Wörtern: für host und client

    srand(time(NULL));   // should only be called once

    int laenge=32;
    String host[laenge]={"katzen","ziegel","ratten","sabbat","tabbak","hamster","laptop","hufen", "Malerei","Fenster" //10
                         "computer","schwimmbad","badezimmer","wort","trampolin","controller","hebel","knebeln","uniprojekt","brille", //10
                         "papier","schere", "steigen","wasser","pflanzen","panzer","adapter","kabelsalat","auflauf","rechnen","aufgeben","rattern"}; //12
 //TODO starte zweiten Timer. Wenn Timer abläuft zufallszahl zurückgeben.

 //TODO oberfläche anzeigen

    int wortid= rand()%laenge;
    uguiptr->PutString(4, 100, host[wortid]);
    uguiptr->PutString(4, 4, "A=OK \nB=neues Wort");


 return 21; //testrückgabe


}

void start(){
    //Variablen initialisieren
    bool ishost=false;
    sender.gpioMode(GPIO::OUTPUT);
    empfaenger.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    sender.gpioWrite(false);
    empfaenger.gpioWrite(false);

    //Ladebildschirm
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
        uguiptr = &gui;

    lcd.clearScreen(0x0);
    gui.SetForecolor(C_ORANGE);
    gui.FontSelect(&FONT_8X12);
    gui.PutString(4, 64, "warten auf Gegner...");

    //Wenn kein Gegenspieler wartet wird der Takt gestartet
    if (!empfaenger.gpioRead()){
        //sender + empfaenger
            taktsender.gpioMode(GPIO::OUTPUT);
        startTakt();
        ishost=true;
    }
    else{
        taktsender.gpioMode(GPIO::INPUT | GPIO::PULLDOWN);
    }


    //Gegenspieler symbolisieren, dass man Ready ist
    WriteBool(true);

    //auf Gegenspieler warten
    while(!empfaenger.gpioRead()){}
    WriteBool(false);


    // Verbunden...
    lcd.clearScreen(0x0);
    gui.SetForecolor(C_ORANGE);
    gui.FontSelect(&FONT_8X12);
    gui.PutString(4, 64, "Verbunden!");
    for (int i = 0; i < 1500000; ++i){}
    lcd.clearScreen(0x0);

    //TODO Begriffsauswahl
    int nummer=begriff(ishost);
    int gegnernummer=-1;

    if(ishost){
        WriteBool(true);
        gegnernummer=leseZahl();
        WriteBool(false);
        sendeZahl(nummer);
    }
    else
    {
        while(!empfaenger.gpioRead()){}
        sendeZahl(nummer);
        while(empfaenger.gpioRead()){}
        gegnernummer=leseZahl();
    }
    WriteBool(false);
	
	Hangwomen_GUI hangwomen;
    hangwomen.start("Wasserrate", &lcd, &gui);


    //TODO interrupt auf den empfänger einrichten

    //TODO Gui anzeigen
    //Raten lassen

    //TODO Wenn fertig, dann 1,1 senden
    //TODO wenn verloren dann 1,0 senden




}
int leseZahl(){
    while(!empfaenger.gpioRead()){}
    //lesen
    int teiler=16;
    int zahl=0;
    for(int i=0;i<5;i++){
       if (ReadBool())
           zahl+=teiler;
       teiler=teiler/2;
    }
    if (ReadBool()){
        leseerror();
    }
    return zahl;

}

void leseerror(){

    gpio_msp432_pin led1( PORT_PIN(1,0) ); // Left red LED
    led1.gpioMode( GPIO::OUTPUT );
    led1.gpioWrite(true);

}

int main(void)
{

    //leds
    gpio_msp432_pin led1( PORT_PIN(1,0) ); // Left red LED
    gpio_msp432_pin led2( PORT_PIN(2,0) ); // red RGB LED
    led1.gpioMode( GPIO::OUTPUT );
    led2.gpioMode( GPIO::OUTPUT );


    start();







    while(true){

    }
}
