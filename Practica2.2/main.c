// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 2.2
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 5, 2018

// I/O List
//Inputs************************************************************************
//Botones en RA5 y RA7

//Outputs*************************************************************
//Leds en RC4 y RC6

// 1. Sección Directivas pre-proceso
// Incluir librerías
#include "config.h"
#include <xc.h>  //Esta es la librería principal del compilador
//en ella se encuentran todas las definiciones para el microcontrolador utilizado
// Definiciones
#define _XTAL_FREQ 32000000
// Macros


// 2. Sección de declaraciones
//   Variables Globales


//   Prototipos de funciones



// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C

void main(void) {
    //Oscilador 32MHz OSC=HF1NT   DIV 1:1
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Configuración de puertos
    PORTA=0; //Limpiar puerto A
    PORTC=0; //Limpiar puerto C
    ANSELA=0; //Puerto A como digital
    ANSELC=0; //Puerto C como digital
    TRISCbits.TRISC4=0; //Puerto RC4 como salida
    TRISCbits.TRISC6=0; //Puerto RC6 como salida
    TRISAbits.TRISA5=1; //Puerto RA5 como entrada
    TRISAbits.TRISA7=1; //Puerto  RA7 como entrada
    WPUAbits.WPUA5=1; //Habilita resistencia Pull-UP en RA5
    WPUAbits.WPUA7=1; //Habilita resistencia Pull-UP en RA7
    //Ciclo infinito
    while(1){
        if (PORTAbits.RA5==0){
            __delay_ms(250);
            LATCbits.LATC6 = LATCbits.LATC6 + 1; //Toggle RC6
        }
        if (PORTAbits.RA7==0){
            __delay_ms(250);
            LATCbits.LATC4 = LATCbits.LATC4 + 1; //Toggle RC4
        }
    }
    return;
}
//Subrutinas
