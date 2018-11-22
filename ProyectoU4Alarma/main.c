// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Proyecto U4: Alarma
// Autores: Javier Camacho Pérez, Ramcés Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Noviembre 21, 2018

// I/O List
//Inputs************************************************************************
//RTC DS3231
//Boton en RC5 y RC7
//Outputs*************************************************************
//Buzer en RC2
//LCD

// 1. Sección Directivas pre-proceso
#include <xc.h> 
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 32000000
#include "flex_lcd.h"

// 2. Sección de declaraciones
//   Variables Globales
int s,m,h,d,mes,a;
int s1,m1,h1,h2=11,m2=33,s2,x,d1,mes1,a1;
 char buffer1[16];
//   Prototipos de funciones



// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) {
     //Oscilador 32MHz OSC=HF1NT   DIV 1:1**************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos*******************************************************************
    PORTA=0;//Limpiamos puerto A
    PORTC=0;//Limpiamos puerto C
    ANSELA=0;//Puerto A como digital
    ANSELC=0;//Puerto C como digital
    TRISC2=0;//RC2 como salida
    TRISC7=1;//RC7 como entrada
    TRISC5=1;//RC5 como entrada
    WPUE3=1;//Resistencia Pull-Up habilitada en Master Clear
    WPUC4=1;//Resistencia Pull Up habilitada en RC4 (Datos))
    WPUC5=1;//Resistencia Pull Up habilitada en RC5
    WPUC7=1;//Resistencia Pull Up habilitada en RC7
    //PPS***********************************************************************
    SSP1CLKPPS=0x13;//Reloj-->RC3
    SSP1DATPPS=0x14;//Datos-->RC4
    RC3PPS=0x14;//Reloj-->RC3
    RC4PPS=0x15;//Datos-->RC4
    //Configuración*************************************************************
    SSP1CON1bits.SSPEN=1;//Habilita puerto serie
    SSP1CON1bits.SSPM=0b1000;//Modo I2C maestro    reloj=FOSC/(4*(SSPADD+1))
    SSP1ADDbits.SSPADD=19; //400KHz reloj
    //LCD***********************************************************************
    Lcd_Init();     //Inicializamos el lcd
    Lcd_Cmd(LCD_CLEAR);		//Limpiamos lcd
    Lcd_Cmd(LCD_CURSOR_OFF);	//Apagamos el cursor
    //RTC**********************************************************************
    SSP1CON2bits.SEN=1;//Generamos señal de start
    while(SSP1CON2bits.SEN);//Esperamos a que se mande
    //Enviamos datos
    /*
    SSP1BUF=0b11010000; //dirección 11010000 y ultimo 0=escritura
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0;//Dirección de la memoria
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0;//0 Segundos
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0b00100110;//45 Minutos
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0b00100011;//23 Horas
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0b00000001;//Lunes
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0b00011001;//19 Día
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1BUF=0b00011000;//2018 Año
    while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
    while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
    SSP1CON2bits.PEN=1;
    while(SSP1CON2bits.PEN);
    */
    while(1){//Ciclo infinito
        SSP1CON2bits.SEN=1;//Generamos señal de start
        while(SSP1CON2bits.SEN);//Esperamos a que se mande
        SSP1BUF=0b11010000; //dirección 11010000 y ultimo 0=escritura
        while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
        while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
        SSP1BUF=0;//Dirección 0 de la memoria
        while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
        while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock 
        //Lectura
        SSP1CON2bits.RSEN=1;//Generamos señal de restart
        while(SSP1CON2bits.RSEN);//Esperamos a que se mande
        SSP1BUF=0b11010001; //dirección 11010000 y ultimo 1=lectura
        while(SSP1STATbits.BF);//Esperamos a que se envíe el dato
        while(SSP1STATbits.R_nW);//Esperamos a que llegue el 9° clock
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        s=SSP1BUF; //Tomamos el valor del segundo
        SSP1CON2bits.ACKDT=0;
        SSP1CON2bits.ACKEN=1;//Generamos ACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el ACK
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        m=SSP1BUF;//Tomamos el valor del minuto
        SSP1CON2bits.ACKDT=0;
        SSP1CON2bits.ACKEN=1;//Generamos ACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el ACK
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        h=SSP1BUF;//Tomamos el valor de la hora
        SSP1CON2bits.ACKDT=0;
        SSP1CON2bits.ACKEN=1;//Generamos ACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el ACK
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        d=SSP1BUF;//Tomamos el valor del dia
        SSP1CON2bits.ACKDT=0;
        SSP1CON2bits.ACKEN=1;//Generamos ACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el ACK
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        mes=SSP1BUF;//Tomamos el valor del mes
        SSP1CON2bits.ACKDT=0;
        SSP1CON2bits.ACKEN=1;//Generamos ACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el ACK
        SSP1CON2bits.RCEN=1;//Habilitamos recepción
        while(SSP1STATbits.BF==0);//Esperamos a que se llene el dato
        a=SSP1BUF;//Tomamos el valor del año
        SSP1CON2bits.ACKDT=1;
        SSP1CON2bits.ACKEN=1;//Generamos NOACK
        while(SSP1CON2bits.ACKEN);//Esperamos a que se mande el NOACK
        SSP1CON2bits.PEN=1;//Generamos señal de stop
        while(SSP1CON2bits.PEN);//Esperamos a que se mande
        s1=((0b01110000 & s)>>4)*10+(0b00001111 & s);//Segundo
        m1=((0b01110000 & m)>>4)*10+(0b00001111 & m);//Minuto
        h1=((0b01110000 & h)>>4)*10+(0b00001111 & h);//Hora
        d1=d;//Día
        mes1=((0b00010000 & mes)>>4)*10 + (0b00001111 & mes);//Dia del mes
        a1=(((0b11110000 & a)>>4)*10 + (0b00001111 & a)) + 2000;//Año
        if (RC5==0){//Si se presiona botón aumentan los minutos de la alarma
            m2++;
            if(m2>=60){m2=0;}
        }
        if (RC7==0){//Si se presiona botón aumenta las horas de la alarma
            h2++;
            if(h2>=24){h2=0;}
        }
        sprintf(buffer1,"   %02d:%02d:%02d",h1,m1,s1);//Imprimimos hora actual
        Lcd_Out2(1, 1, buffer1);
        sprintf(buffer1,"   %02d:%02d:%02d",h2,m2,s2);//Imprimimos alarma
        Lcd_Out2(2, 1, buffer1);
        __delay_ms(900);//Cada 900 ms
        if(h1==h2 && m1==m2){//Si coincide hora y minuto la alarma suena por 30 segundos 
            x++;
            if (x<30){LATC2=LATC2+1;}
            if (x>=30){LATC2=0;}
        }
        if(h1!=h2 || m1!=m2){//Si no coincide se apaga 
            LATC2=0;
            x=0;
        }
    }
    return;
}


