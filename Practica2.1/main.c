// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 2.1
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 3, 2018

// I/O List
//Inputs************************************************************************

//Outputs***********************************************************************
// LED en RC6

// 1. Sección Directivas pre-proceso
// Incluir librerías
#include <xc.h>
#include <pic16f18877.h>  //Esta es la librería principal del compilador
//en ella se encuentran todas las definiciones para el microcontrolador utilizado
#include "config.h"

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
    PORTC=0; //PORTC=0b00000000; PORTC=0x00; //Limpiar puerto C
    ANSELC=0; //Puerto C como digital
    TRISCbits.TRISC6=0; //Puerto RC6 como salida
    
   //Ciclo infinito
    
    while(1){
        LATCbits.LATC6=LATCbits.LATC6+1; //Toggle RC6
        __delay_ms(500);
    }
    return;
}

// Subrutinas 
