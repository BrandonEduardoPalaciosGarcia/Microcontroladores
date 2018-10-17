// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 4.2
// Autores: Javier Camacho Pérez, Ramces Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Octubre 17, 2018

// I/O List
//Inputs************************************************************************
//Recepción de datos por Comunicación Serial
//Potenciómetro en RC3
//Outputs*************************************************************
//Relevadores en RD4 y RD5
//Transmisión de datos por Comunicación Serial

// 1. Sección Directivas pre-proceso
#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000

// 2. Sección de declaraciones
//   Variables Globales
int adc;
char dato;
//   Prototipos de funciones

// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) { 
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos************************************************************
    PORTD=0;//Limpia puerto D
    ANSELD=0;//Puerto D como digital
    TRISD=0;//Puerto D como salida
    PORTC=0;//Limpia puerto C
    ANSELC=0b00001000;//Puerto C como digital y RC3 como análogico
    TRISC7=1;//Pin RC7 como entrada
    TRISC6=0;//Pin RC6 como salida
    TRISC3=1;//Pin RC3 como entrada
    WPUE3=1;//Resistencia Pull-Up habilitada en RE3
    //PPS*********************************************************
    RXPPS=0x17;//Rx---->RC7
    RC6PPS=0x10;//TX----->RC6
    //Convertidor ADC************************************************
    ADCON0bits.ADON=1; //Habilitar ADC
    ADCON0bits.ADCS=1; //Seleccionar oscilador interno
    ADREFbits.ADNREF=0; //Tierra
    ADREFbits.ADPREF=0b00; //Voltaje de alimentación
    ADCON0bits.ADFRM0=1;//Justificación derecha
    //Puerto serie************************************************
    RC1STAbits.SPEN=1;//Habilita puerto serie
    TX1STAbits.BRGH=0;//Low speed
    TX1STAbits.SYNC=0;//Asincrono
    RC1STAbits.CREN=1;//Recepción continua
    TX1STAbits.TXEN=1;//Habilita TX
    //BAUD rate***************************************************
    BAUD1CONbits.BRG16=0;//Timer USART 8 bits
    SPBRG=51;//9600 Bauds
    //Interrupcion***********************************************
    PIR3bits.RCIF=0;//Limpia bandera INT Rx
    PIE3bits.RCIE=1;//Habilita INT Rx
    INTCONbits.PEIE=1;//Habilita interrupción por periférico
    INTCONbits.GIE=1;//Interrupción global habilitada
    //Ciclo infinito*****************************************************
    while(1){
        ADPCH=0b010011;//Canal ANC3
        __delay_us(20);//Carga del capacitor
        ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversion
        adc=ADRES;//Iguala variable de adc al registro ADRES para obtener valores de 0 a 1023
        adc=adc/4;//Divide variable entre 4 para tenerla en 8 bits
        TX1REG=adc;//Transmite valor de adc
        while(TX1STAbits.TRMT==0);//Espera hasta que se envíe el dato
        __delay_ms(300);//Cada 300ms      
    }
    return;
}
//Aquí se define la subrutina
void __interrupt() isr(void){
    dato=RC1REG;//Lee dato recibido
    if (dato==65){LATD4=LATD4+1;}//Activa y desactiva relevador en RD4
    if (dato==66){LATD5=LATD5+1;}//Activa y desactiva relevador en RD5
    PIR3bits.RCIF=0;//Limpia la bandera
}
