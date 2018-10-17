// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 3.5
// Autores: Javier Camacho Pérez, Ramces Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Octubre 11, 2018

// I/O List
//Inputs************************************************************************
//Sensor ultrasónico en RD0 y RD1
//Outputs*************************************************************
//LCD para mostrar distancia

// 1. Sección Directivas pre-proceso
#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000
#include "LCD.h"
#include <stdio.h>


// 2. Sección de declaraciones
//   Variables Globales
int distancia;
float t;
unsigned char buffer1[16];
//   Prototipos de funciones

// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) { 
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos************************************************************
    ANSELA=0;//Puerto A como digital
    PORTD=0;//Limpia puerto D
    TRISDbits.TRISD1=1;//Pin RD1 como entrada
    TRISDbits.TRISD0=0;//Pin RD0 como salida
    ANSELD=0;//Puerto D como digital
    WPUEbits.WPUE3=1;//Resistencia habilitada en RE3
    //Timer**************************************************************
    T1CLKbits.CS=0b0001;//Fosc/4
    T1CONbits.RD16=1;//TMRH buffered
    T1CONbits.CKPS=0b11;//Prescaler 1:8
    T1CONbits.ON=1;//Habilitar Timer 1
    //LCD****************************************************************
    Lcd_Init(); //Inicializar la LCD
    Lcd_Cmd(LCD_CLEAR); //Limpiar la LCD
    Lcd_Cmd(12);//Apagar cursor
    __delay_ms(100);//Espera 100ms
    //Ciclo infinito*****************************************************
    while(1){
        TMR1=0;//Limpiamos TMR1
        LATD0=1;//Activa trigger
        __delay_us(10);
        LATD0=0;//Desactiva trigger
        while(RD1==0);//Espera hasta que RD1 reciba diferente de 0
        TMR1=0;//Limpiamos TMR1
        while(RD1==1);//Espera hasta que RD1 reciba diferente de 1 y la cuenta en TMR1 empieza
        t=TMR1*0.000001;//Tiempo en regresar al sensor (TMR1*Tick)
        distancia=(t*34000)/2;//Calculo de la distancia según la velocidad del sonido
        if (distancia>=0 && distancia <=300){//Solo imprimimos valores entre 0 y 300
        sprintf(buffer1,"Dist(cm)= %03d",distancia);//Imprimir Distancia en Fila 1
        Lcd_Out2(1,1,buffer1);
        __delay_ms(100);}//Esperamos 100ms
    }
    return;
}
//Aquí se define la subrutina
