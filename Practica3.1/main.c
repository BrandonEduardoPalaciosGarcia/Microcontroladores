// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 3.1 
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 24, 2018

// I/O List
//Inputs************************************************************************
//Dos pulsadores con lógica negativa en RB0 y RB7
//Outputs***********************************************************************
//Leds en RD1 y RD2

// 1. Sección Directivas pre-proceso
// Incluir librerías
#include <xc.h>
#include "config.h"

// Definiciones
#define _XTAL_FREQ 16000000
// Macros


// 2. Sección de declaraciones
//   Variables Globales



//   Prototipos de funciones
void __interrupt () Interrupcion(void){
    //Interrupción por RB0 IOC
    if (IOCBFbits.IOCBF0)
    {
       __delay_ms(250); //Delay Anti rebote
       LATD1=LATD1+1; //Cambiar estado de RD1
       IOCBFbits.IOCBF0=0; //Limpiamos bandera de interrupción en RB0
       PIR0bits.IOCIF=0; //Limpiar bandera IOC
    }
    //Interrupción por RB7 INT
    if (PIR0bits.INTF)
    {
       __delay_ms(250);//Delay Anti Rebote
       LATD2=LATD2+1;//Cambiar estado de RD2
       PIR0bits.INTF=0;//Limpia la bandera
    } 
}

// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) {
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b101; //HFINTOSC 16 MHz
    //Puertos************************************************************
    PORTB=0;    //Limpia puerto B
    PORTD=0;    //Limpia el puerto D
    ANSELB=0;   //Puerto B como digital
    TRISB=0xFF; //Puerto B como entradas
    TRISD=0;    //Puerto D como salidas
    ANSELD=0;   //Puerto D como digital
    WPUB=255;   //Resistencia pull Up internas habilitadas en puerto B
    WPUEbits.WPUE3=1; //Habilita resistencia Pull UP en RE3
    //Peripheral Pin Selec
    INTPPS=0x0F;//INT---->RB7
    //Interrupciones*****************************************************
    INTCONbits.GIE=1;  //Interrupción global habilitada
    //Interrupcion INT
    INTCONbits.INTEDG=1;//Flanco de subida
    PIR0bits.INTF=0;//Limpia bandera INT
    PIE0bits.INTE=1;//Habilita interrupcion INT
    //Interrupcion IOC
    PIE0bits.IOCIE=1; //Habilitamos interrupción IOC
    PIR0bits.IOCIF=0; //Limpiamos bandera
    IOCBPbits.IOCBP0=1; //Interrupción por flanco de subida en RB0
    IOCBNbits.IOCBN0=1; //Interrupción por flanco de bajada en RB0
    IOCBF=0; //Limpia las banderas del puerto B
//4. Inicia ciclo infinito
    while(1);
    return;
}

// Subrutinas 
