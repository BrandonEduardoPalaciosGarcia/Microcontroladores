// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 2.5
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 10, 2018

// I/O List
//Inputs************************************************************************
//Potenciómetro en D0 y Sensor Lm35 en D1

//Outputs***********************************************************************
//LCD

// Incluir librerías y Definiciones*********************************************


#include <xc.h>
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"

//Main**************************************************************************
void main(void) {
    //Declaración de variables**************************************************
    int adc=0;
    float volt=0,temp=0;
    unsigned char buffer1[16];
     //Oscilador 32MHz OSC=HF1NT   DIV 1:1**************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos*******************************************************************
    ANSELA=0;//Puerto A como digital
    ANSELDbits.ANSD0=1; //RD0 como analógico
    TRISDbits.TRISD0=1; //RD0 como entrada
    ANSELDbits.ANSD1 =1; //RD1 como analogico
    TRISDbits.TRISD1=1; //RD1 como entrada 
    //Convertidor ADC************************************************
    ADCON0bits.ADON=1; //Habilitar ADC
    ADCON0bits.ADCS=1; //Seleccionar oscilador interno
    ADREFbits.ADNREF=0; //Tierra
    ADREFbits.ADPREF=0b00; //Voltaje de alimentación
    ADCON0bits.ADFRM0=1;//Justificación derecha
    //LCD************************************************************
    Lcd_Init();//Inicializar LCD
    Lcd_Cmd(LCD_CLEAR );//Limpiar LCD
    Lcd_Cmd(LCD_CURSOR_OFF );//Apagar el cursor
    __delay_ms(100);
    //Ciclo Infinito************************************************
    while(1){
        ADPCH=0b011001;//Canal AND1
        __delay_us(20);//Carga del capacitor
        ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversion
        adc=(ADRESH<<8)+ADRESL;//Guardar valor del ADC
        temp=(adc*5)/10.23;//Conversion del valor del ADC a Temperatura
        ADPCH=0b011000;//Canal AND0
        __delay_us(20);//Carga del capacitor
        ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversión
        adc=(ADRESH<<8)+ADRESL;//Guardar valor del ADC
        volt=(adc*5)/1023.0; //Conversion a voltaje
        sprintf(buffer1,"Voltaje= %0.2f",volt);//Imprimir Voltaje en Fila 1
        Lcd_Out2(1,1,buffer1);
        sprintf(buffer1,"Temp(C)= %0.1f",temp);//Imprimir temperatura en Fila 2
        Lcd_Out2(2,1,buffer1);
        __delay_ms(300);//Repite cada 300ms
    }
    return;
}
