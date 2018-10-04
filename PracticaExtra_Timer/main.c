#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000
void main(void) {
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos********************************************
    PORTB=0; //Limpiar puerto B
    TRISBbits.TRISB5=0; // RB5 como salida
    ANSELBbits.ANSB5=0; //RB5 como digital
    WPUEbits.WPUE3=1; //Habilita resistencia Pull UP en RE3
    //PPS***********************************************
    RB5PPS=0x18; //Salida del TMR0 en RB5
    //Timer 0 ******************************************************
    T0CON0bits.T0EN=1; //Habilita Timer 0
    T0CON0bits.T016BIT=0; //Timer de 8 bits
    T0CON0bits.T0OUTPS=0b1111; //Postscaler 1:16
    T0CON1bits.T0CS=0b010; //Fosc/4
    T0CON1bits.T0ASYNC=0; //Sincronización con Fosc/4
    T0CON1bits.T0CKPS=0b1100; //Prescaler 1:4096
    TMR0L=122;
    while(1);
    return;
}
