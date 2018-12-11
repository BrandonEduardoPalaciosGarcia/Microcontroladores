// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 5.1 Medición con Encoder
// Autores: Javier Camacho Pérez, Ramcés Mohamed Hernández Romero, Brandon Eduardo Palacios García
// Fecha: Noviembre 21, 2018

// I/O List
//Inputs************************************************************************
//Encoder KY-040
//Outputs*************************************************************
//Display 7 segmentos 4 digítos

// 1. Sección Directivas pre-proceso
#include <xc.h> 
#include "config.h"
#include <pic16f18877.h>
#define _XTAL_FREQ 32000000

// 2. Sección de declaraciones
//   Variables Globales
char dir='A';
int P=0;
float dis,circ=0.075398,disa,decimal;
int distancia,d,u,dd,ud,x,y;
char buffer1[16];
//   Prototipos de funciones
void Display(void);
void Cambio (void);
//Main*************************************************************************
void main(void) {
    //Puertos******************************************************************
    PORTA=0;//Limpia puerto A
    PORTB=0;//Limpia puerto B
    PORTC=0;//Limpia puerto C
    PORTD=0;//Limpia puerto D
    ANSELA=0;//Puerto C como digital
    ANSELB=0;//Puerto B como digital
    ANSELC=0;//Puerto C como digital
    ANSELD=0;//Puerto C como digital
    TRISA0=1;//RA0 como entrada
    TRISB=255;//Puerto B como entrada
    TRISC=0;//Puerto C como salida
    TRISD=0;//Puerto D como salida
    WPUE3=1;//Resistencia Pull Up habilitada en RE3
    WPUB2=1;//Resistencia Pull Up habilitada en RB2
    WPUA0=1;//Resistencia Pull Up habilitada en RA0
    //Interrupciones***********************************************************
    PIE0bits.IOCIE=1; //Habilitamos interrupción IOC
    PIR0bits.IOCIF=0; //Limpiamos bandera IOC
    IOCBN0=1;//Interrupción por flanco de bajada en RB0
    IOCBN2=1;//Interrupción por flanco de bajada en RB2
    IOCAN0=1;//interrupción por flanco de bajada en RA0
    IOCAF=0;//Limpiamos banderas del puerto A
    IOCBF=0;//Limpiamos banderas del puerto B
    INTCONbits.GIE=1;//Interrupción global habilitada
    __delay_ms(100);//Esperamos 100 ms
    while(1){
        dis=(circ/20)*P;//Calculamos distancia a partir de la circunferencia de la rueda y la posición
        if (dis>=99.99){//Si supera 99.99 inicia en 0
            P=0;
            dis=0;
        }
        distancia=dis;//Tomamos la parte entera de dis
        d=distancia/10;//Separamos las decenas de la parte entera
        x=d;//x toma el valor para mostrarlo en los displays
        y=1;//Dígito 1
        Cambio();
        Display();
        u=distancia-(d*10);//Separamos las unidades de la parte entera
        x=u;//x toma el valor para mostrarlo en los displays
        y=2;//Digito 2
        Cambio();
        Display();
        decimal=dis-distancia;//Tomamos la parte decimal del número
        disa=decimal*100;//Convertimos la parte decimal a entero de dos dígitos
        dd=disa/10;//Separamos las decenas de la parte decimal
        x=dd;//x toma el valor para mostrarlo en los displays
        y=3;//Digito 3
        Cambio();
        Display();
        ud=disa-(dd*10);//Separamos las unidades de la parte decimal
        x=ud;//x toma el valor para mostrarlo en los displays
        y=4;//Digito 4
        Cambio();
        Display();
    }
    return;
}
void __interrupt () ISR(void){
        if (IOCBF0==1){//Se activa interrupción de Encoder
            if(dir=='A'){//Si  dir es A la Posición aumenta hacia adelante
            if (RB1==1){//Verificamos si va hacia adelante o hacia atrás
                P++;
            }
            }
            if(dir=='B'){//Si  dir es B la Posición decrementa hacia atrás
            if(RB1==0){
                P--;
                if (P<=0){P=0;}
            }  
            }
        IOCBF0=0;//Limpiamos bandera en RB0
        __delay_ms(1);//Esperamos 1 ms
        }
        if (IOCBF2==1){//Si presionamos el eje del encoder limpiamos posición
            P=0;
            IOCBF2=0;//Limpiamos bandera en RB2
        }
        if (IOCAF0==1){//Si presionamos Botón cambia dir
            __delay_ms(5);
            if(dir=='A'){//Si es A cambia a B
                dir='B';
            }
            else if(dir=='B'){//Si es B cambia a A
                dir='A';
            }
            IOCAF0=0;//Limpiamos bandera en RA0
        }
        PIR0bits.IOCIF=0; //Limpiamos bandera IOC
}
void Display(void){//Función para mostrar números en Display
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
        __delay_ms(2);//Tiempo para observar digítos distintos en los displays  
}

void Cambio(void){//Función para cambiar Displays
    switch(y){
        case 1:
            LATD=0b10111011;//Enciende Display 1
        break;
        case 2:
            LATD=0b11110111;//Enciende Display 2 y punto decimal
        break;
        case 3:
            LATD=0b10101111;//Enciende Display 3 
        break;
        case 4:
            LATD=0b10011111;//Enciende Display 4
        break;
    }
}
