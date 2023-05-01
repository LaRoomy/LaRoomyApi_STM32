#ifndef _PORTENTA_LED_CONTROL_
#define _PORTENTA_LED_CONTROL_

#include <Arduino.h>

enum PortentaRGBColor {
    LEDOFF,
    REDCOL,
    GREENCOL,
    BLUECOL,
    YELLOWCOL,
    PURPLECOL,
    CYANCOL,
    WHITECOL
};

void initRGBLed(){
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
}

void setRGBLedColorD(PortentaRGBColor color){
    switch(color){
        case PortentaRGBColor::LEDOFF:
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, HIGH);
            break;
        case PortentaRGBColor::REDCOL:
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, HIGH);
            break;
        case PortentaRGBColor::GREENCOL:
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, HIGH);
            break;
        case PortentaRGBColor::BLUECOL:
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, LOW);
            break;
        case PortentaRGBColor::YELLOWCOL:
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, HIGH);
            break;
        case PortentaRGBColor::PURPLECOL:
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, HIGH);
            digitalWrite(LEDB, LOW);
            break;
        case PortentaRGBColor::CYANCOL:
            digitalWrite(LEDR, HIGH);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);
            break;
        case PortentaRGBColor::WHITECOL:
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);
            break;
        default:
            break;      
    }
}

#endif // _PORTENTA_LED_CONTROL_