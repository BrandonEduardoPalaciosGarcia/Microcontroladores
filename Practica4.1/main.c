#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000
int x;
void main(void) {
    //Oscilador**********************************************************
    OSCFRQbits.HFFRQ=0b110; //HFINTOSC 32 MHz
    //Puertos************************************************************
    WPUE3=1;//Resistencia Pull-Up habilitada en RE3
    PORTC=0;//Limpia puerto C
    ANSELC=0;//Puerto C como digital
    TRISC=0;//Puerto C como salida
    PORTD=0;//Limpia puerto D
    ANSELD=255;//Puerto D como análogico
    TRISD=255;//Puerto D como entrada
    //PPS****************************************************************
    RC3PPS=0x09;//CCP1--->RC3
    //ADC****************************************************************
    ADCON0bits.ADON=1; //Habilitar ADC
    ADCON0bits.ADCS=1; //Seleccionar oscilador interno
    ADREFbits.ADNREF=0; //Tierra
    ADREFbits.ADPREF=0b00; //Voltaje de alimentación
    ADCON0bits.ADFRM0=1;//Justificación derecha
    //Timer 2--->CCP1****************************************************
    PR2=255;//Señal PWM 1.95 kHz
    T2CLKCON=0b0001;//CS como Fosc/4
    T2CONbits.CKPS=0b100;//Prescaler 1:16
    T2CONbits.OUTPS=0;//Posescaleer 1:1
    T2CONbits.ON=1;//Habilita Timer 2
    //CCP1***************************************************************
    CCP1CONbits.EN=1;//Habilita módulo CCP1
    CCP1CONbits.FMT=0;//Alineación a la derecha
    CCP1CONbits.MODE=0b1111;//Módulo CCP1 como PWM
    CCPTMRS0bits.C1TSEL=0b01;//Timer 2---->CCP1
    TRISC=0;//Puerto B como Salidas
    //Ciclo infinito*****************************************************
    while(1){
        ADPCH=0b011001;//Canal AND1
        __delay_us(20);//Carga del capacitor
        ADCON0bits.GO=1;//Inicia la conversión
        while(ADCON0bits.GO);//Espera a que termine la conversion
        x=ADRES;//Iguala variable de adc al registro ADRES para obtener valores de 0 a 1023
        if (x>512){
            LATC2=0;
            CCPR1=(x-512)*2;
        }
        if (x<512){
            LATC2=1;
            CCPR1=(x-512)*2;
        }

    }
    return;
}
