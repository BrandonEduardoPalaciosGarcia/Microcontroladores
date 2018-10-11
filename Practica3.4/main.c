// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 3.4 
// Autores: Javier Camacho Pérez, Ramces Mohamed Hernández Romero
// Fecha: Octubre 10, 2018

// I/O List
//Inputs************************************************************************
//SW con lógica negativa para cambiar entre las frecuencias
//Outputs*************************************************************
//Led en RA0


// 1. Sección Directivas pre-proceso
#include <xc.h>
#include "config.h"
#define _XTAL_FREQ 32000000

// 2. Sección de declaraciones
//   Variables Globales
int selec=0;

//   Prototipos de funciones



// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) {
    //Ports**************************
    TRISA=0b11110000;//Puerto A como entradas y salidas
    ANSELA=0;//Puerto A como digital
    PORTA=0;//Limpiar Puerto A
    WPUA=0b11110000;//Resistencia Pull-Up activas en RA7 RA6 RA5 RA4
 
    //Interrupt**********************
    PIR4bits.TMR1IF=0; //Limpia bandera Timer 1
    PIE4bits.TMR1IE=1; //Habilita interrupcion por Timer1
    INTCONbits.PEIE=1; //Habilita interrupcion por periferico
    
    PIR0bits.IOCIF=0; //Limpia banera interrupcion IOC
    IOCAN=0b11110000; //Flanco de bajada 
    IOCAF=0; //Limpia banderas puerto A
    PIE0bits.IOCIE=1; // Habilita interrupcion IOC
     
    INTCONbits.GIE=1; //Habilita interrupcion global
    
    //Timer 1************************
    T1CONbits.CKPS=0b11; //Prescaler 1:8
    T1CONbits.RD16=1; //
    T1CLKbits.CS=0b0001; //Fosc/4
    TMR1=15536; //Precarga
    T1CONbits.ON=1; //Enciende Timer 1
    
    while(1);
    return;
}

// Subrutinas
void __interrupt() ISR(void)
{
    if(IOCAFbits.IOCAF7){ //Cambia el estado de selec cada que pesiona el boton
        selec=~selec;  //Selec negado
    }
    
    if(PIR4bits.TMR1IF){ //Cada que el timer1 se desborde
        
      if(selec){  //Selec cambia la frecuencia     
        LATA0=LATA0+1; //Cambia el estado de A0
        TMR1=60536; //Pecarga para f=100Hz
      }
      else{
        LATA0=LATA0+1; //Cambia el estado de A0
        TMR1=15536; //Precarga para f=10Hz
      }
    
    }
    
      PIR4bits.TMR1IF=0; //Lumpia la bandera del Timer1
      IOCAFbits.IOCAF7=0; //Limpia bandera puerto A7
      PIR0bits.IOCIF=0; //Limpia la bandera de interrupcion IOC
   
}
