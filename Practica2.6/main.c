// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 2.6
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramcés Mohamed Hernández Romero
// Fecha: Septiembre 17, 2018

// I/O List
//Inputs************************************************************************
//Potenciómetro en D0
//Potenciómetro en C3
//Sensor Lm35 en D1

//Outputs***********************************************************************
//LCD
//Relevador en D2 conectado al foco
//Relevador en D3 conectado al ventilador

// Incluir librerías y Definiciones*********************************************


#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <pic16f18877.h>
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"

//Main**************************************************************************
void main(void) {
    //Declaración de variables**************************************************
    int adc=0, hist=0, ref=0;
    float temp=0;
    unsigned char buffer1[16];
     //Oscilador 32MHz OSC=HF1NT   DIV 1:1**************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos*******************************************************************
    PORTA=0;//Limpiar puerto A
    PORTC=0;//Limpiar puerto C
    PORTD=0;//Limpiar puerto D
    ANSELA=0;//Puerto A como digital
    ANSELDbits.ANSD0=1; //RD0 como analógico
    TRISDbits.TRISD0=1; //RD0 como entrada
    ANSELDbits.ANSD1 =1; //RD1 como analogico
    TRISDbits.TRISD1=1; //RD1 como entrada 
    ANSELDbits.ANSD2=0; //RD2 como digital
    TRISDbits.TRISD2=0;//RD2 como salida
    ANSELDbits.ANSD3=0; //RD3 como digital
    TRISDbits.TRISD3=0;//RD3 como salida
    ANSELCbits.ANSC3=1;//RC3 como analogico
    TRISCbits.TRISC3=1;//RC3 como entrada
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
        ref=((adc*0.054)+25); //Rango de la referencia entre 25 y 80 
        ADPCH=0b010011;//Canal ANC3
        __delay_us(20);//Carga del capacitor
         ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversion
        adc=(ADRESH<<8)+ADRESL;//Guardar valor del ADC
        hist=(adc*10)/1023.0;//Valores de la histeresis entre 0 y 10
        sprintf(buffer1,"Ref= %02d",ref);//Imprimir Voltaje en Fila 1
        Lcd_Out2(2,1,buffer1);
        sprintf(buffer1,"H= %02d",hist);//Imprimir Histeresis en Fila 1
        Lcd_Out2(2,10,buffer1);
        sprintf(buffer1,"Temp(C)= %0.1f",temp);//Imprimir temperatura en Fila 2
        Lcd_Out2(1,1,buffer1);
        __delay_ms(300);//Repite cada 300ms
        if (temp<(ref-hist)){
            LATDbits.LATD2=1;//Encender foco
            LATDbits.LATD3=0;//Apagar ventilador
        }
        if (temp>(ref+hist)){
            LATDbits.LATD2=0;//Apagar foco
            LATDbits.LATD3=1;//Encender ventilador
        }
    }
    return;
}
