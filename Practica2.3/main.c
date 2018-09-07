// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 2.3
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 6, 2018

// I/O List
//Inputs************************************************************************


//Outputs*************************************************************
//LCD

// Incluir librerías**************************************************
#include <xc.h>
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"
//Definiciones********************************************************



void main(void) {
    int x;
    unsigned char buffer1[16];
    //Oscilador 32MHz OSC=HF1NT   DIV 1:1
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos******************************************
    ANSELD=0; //Puerto D como digital
    ANSELC=0; //Puerto C como digital
    Lcd_Init(); //Inicializar la LCD
    Lcd_Cmd(LCD_CLEAR); //Limpiar la LCD
    Lcd_Cmd(LCD_BLINK_CURSOR_ON );
    __delay_ms(1000);
    Lcd_Cmd(LCD_CURSOR_OFF );
    //Ciclo Infinito
    while(1){
     for(x=0;x<=12;x++){
        Lcd_Out(1,x,"   Mecatronica  ");
        sprintf(buffer1," Contador= %02d",x);      
        Lcd_Out2(2,1,buffer1);
        __delay_ms(500);
    }
    }
    return;
}
