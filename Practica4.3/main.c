// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 4.3
// Autores: Javier Camacho Pérez, Ramces Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Octubre 22, 2018

// I/O List
//Inputs************************************************************************
//Sensor Lm35 de temperatura
//Outputs*************************************************************
//Comunicación serial

// 1. Sección Directivas pre-proceso
#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000

// 2. Sección de declaraciones
//   Variables Globales
int adc;//Variable para mandar los datos

//   Prototipos de funciones

// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) { 
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos************************************************************
    PORTC=0;//Limpia puerto C
    ANSC1=1;//Puerto RC1 como analógico
    ANSC6=0;//RC6 como digital
    TRISC1=1;//RC1 como entrada
    TRISC6=0;//RC6 como salida
    WPUE3=1;//Resistencia Pull-Up habilitada en RE3
    //PPS*********************************************************
    RC6PPS=0x10;//Tx---->RC6
    //Puerto Serie
    RC1STAbits.SPEN=1;//Puerto serie ON
    TX1STAbits.TXEN=1;//Habilita TX
    TX1STAbits.SYNC=0;//No sincronizado
    TX1STAbits.BRGH=0;//Low speed
    //Baud rate**********************************************************
    BAUD1CONbits.BRG16=0;//Timer EUSART 8bits
    SPBRG=51;//9600
    //Convertidor ADC************************************************
    ADCON0bits.ADON=1; //Habilitar ADC
    ADCON0bits.ADCS=1; //Seleccionar oscilador interno
    ADREFbits.ADNREF=0; //Tierra
    ADREFbits.ADPREF=0b00; //Voltaje de alimentación
    ADCON0bits.ADFRM0=1;//Justificación derecha
    //Ciclo infinito*****************************************************
    while(1){
        ADPCH=0b010001;//Canal ANC1
        __delay_us(20);//Carga del capacitor
        ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversion
        adc=ADRES;//Iguala variable de adc al registro ADRES para obtener valores de 0 a 1023
        adc=adc/4;//Divide variable entre 4 para tenerla en 8 bits
        TX1REG=adc;//Transmite valor de x
        while(TX1STAbits.TRMT==0);//Esperar a que se transmita el dato
        __delay_ms(300);//Cada 300 ms
        
    }
    return;
}
//Aquí se define la subrutina
