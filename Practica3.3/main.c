// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 3.3
// Autores: Javier Camacho Pérez, Ramces Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Octubre 1, 2018

// I/O List
//Inputs************************************************************************
//SW con lógica negativa en RA4
//Outputs*************************************************************
//Display de 7 segmentos de 4 Digítos, en el cual sólo usamos dos, conectado al puerto C a través de
//una resistencia y el cátodo común en RD1 y RD3

// 1. Sección Directivas pre-proceso
#include <xc.h>  
#include "config.h"
#define _XTAL_FREQ 32000000


// 2. Sección de declaraciones
//   Variables Globales
int x=0;

//   Prototipos de funciones
int Display(void);
// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) { 
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos************************************************************
    PORTA=0;//Limpiar puerto A
    TRISA=255;//Puerto A como entrada
    ANSELA=0;//Puerto A como digital
    WPUA=255;//Resistencias Pull-Up Activas en RA
    PORTC=0;//Limpiar puerto C
    TRISC=0;//Puerto C como salida
    ANSELC=0;//Puerto C como digital 
    PORTD=0;//Limpiar puerto D
    TRISD=0;//Puerto D como salida
    ANSELD=0;//Puerto D como digital
    WPUEbits.WPUE3=1;//Resistencia Pull-Up Activa en RE3
    //PPS****************************************************************
    T0CKIPPS=0x04; //T0CKI----->RA4
    //Timer 0************************************************************
    T0CON0bits.T016BIT=0;//8 bits Timer
    T0CON0bits.T0OUTPS=0b0000;//Posescaler 1:1
    T0CON1bits.T0CS=0b001;//CS=T0CKIPPS negado
    T0CON1bits.T0ASYNC=1;//No sincronizado
    T0CON1bits.T0CKPS=0b0000;//Prescaler 1:1
    TMR0H=99;//Precarga
    TMR0L=0;//Valor inicial
    T0CON0bits.T0EN=1;//Timer 0 habilitado
    //Ciclo infinito*****************************************************
    while(1){
        x=TMR0L % 10;//Del registro 'x' toma únicamente el valor de las unidades
        LATDbits.LATD1=0;//Activa 4° digito del display
        LATDbits.LATD3=1;//Desactiva 3° digito del display
        Display();//El display muestra el número según 'x'
        x=(TMR0L-x)/10;//Del registro 'x' toma únicamente el valor de las decenas
        LATDbits.LATD1=1;//Desactiva 4° digito del display
        LATDbits.LATD3=0;//Activa 3° digito del display
        Display();//El display muestra el número según 'x'
        PORTC=0;//Limpia puerto C
    }
    return;
}
//Aquí se define la subrutina
int Display (){
        switch(x){
        case 0:
            LATC=0b00111111;//Display muestra '0'
        break;
        case 1:
            LATC=0b00000110;//Display muestra '1'
        break;
        case 2:
            LATC=0b01011011;//Display muestra '2'
        break;
        case 3:
            LATC=0b01001111;//Display muestra '3'
        break;
        case 4:
            LATC=0b01100110;//Display muestra '4'
        break;
        case 5:
            LATC=0b01101101;//Display muestra '5'
        break;
        case 6:
            LATC=0b01111100;//Display muestra '6'
        break;
        case 7:
            LATC=0b00000111;//Display muestra '7'
        break;
        case 8:
            LATC=0b01111111;//Display muestra '8'
        break;
        case 9:
            LATC=0b01100111;//Display muestra '9'
        break;
    }
        __delay_ms(10);//Tiempo para observar digítos distintos en los displays
}
