#include <xc.h>
#include "config.h"
#include <stdio.h>
#define _XTAL_FREQ 32000000

  char dato;
  char cadena[20];
  char receive[1000];
  int num=10;
  int tam=0;
  int i=0;

  
void __interrupt() RX(void)
{
    receive[i] = RC1REG; //Lee el dato recibido
     
    if(receive[i]=='+'){
        i=0;
        receive[i] = RC1REG;
    }
    
     i++;
     
        if(receive[0]=='+' && receive[1]=='I' && receive[2]=='P' && receive[3]=='D'){
        
            if(receive[16]=='A'){
             LATB7=0; //Si el dato es 54 enciende el LED
            }
            
            if(receive[16]=='B'){
             LATB7=1; //Si el dato es 54 enciende el LED
            }
            
             if(receive[16]=='C'){
             LATB6=0; //Si el dato es 54 enciende el LED
            }
            
            if(receive[16]=='D'){
             LATB6=1; //Si el dato es 54 enciende el LED
            }
            
             if(receive[16]=='E'){
             LATB5=0; //Si el dato es 54 enciende el LED
            }
            
            if(receive[16]=='F'){
             LATB5=1; //Si el dato es 54 enciende el LED
            }
            
             if(receive[16]=='G'){
             LATB4=0; //Si el dato es 54 enciende el LED
            }
            
            if(receive[16]=='H'){
             LATB4=1; //Si el dato es 54 enciende el LED
            }
            
     }
       

    PIR3bits.RCIF=0;
    
}

void main(void) {
    
    //Puertos
    PORTC=0;
    ANSELC=0;
    TRISC=0b00010000;
    
    PORTB=0;
    ANSELB=0;
    TRISB=0;
      
    WPUEbits.WPUE3=1;
    
    //PPS
    RC5PPS=0x10; //TX--> RC3
    RXPPS=0x14; //RX-->RC4
    
    //Puerto Serie
    RC1STAbits.SPEN=1; //Puerto serie ON
    TX1STAbits.TXEN=1; //Habilita TX 
    RC1STAbits.CREN=1; //Habilita RX
    TX1STAbits.SYNC=0; //Asincrono
    TX1STAbits.BRGH=1; //Alta velocidad    
    //Baud Rate
    BAUD1CONbits.BRG16=0; //Timer ESUSART 8 bits
    SPBRG=16; //Precarga del Timer --> 115.2k baudios
    
     //Interrupcion
    PIR3bits.RCIF=0; //Limpia bandera INT RX
    PIE3bits.RCIE=1; //Habilita INT RX
    INTCONbits.PEIE=1; //Habilita interrupcion por periferico
    INTCONbits.GIE=1;  //Habilita interrupcion global
    
    LATB7=1;
    LATB6=1;
    LATB5=1;
    LATB4=1;
    
    __delay_ms(1000);
    
    
  
    
    //Comandos AT///////////////////////////////////////////////////////////////
    
 /*
    tam = sprintf(cadena,"AT\r\n");
    
     for(int i=0; i<=tam; i++)
        {
            TX1REG = cadena[i];
            while(TRMT==0);
        }
        __delay_ms(10);

    */
    
    tam = sprintf(cadena,"AT+CWJAP=\"Brandon\"%c\"123456780\"\r\n", ',');
    
     for(int i=0; i<=tam; i++)
        {
            TX1REG = cadena[i];
            while(TRMT==0);
        }
        __delay_ms(1000);
    
    
    tam = sprintf(cadena,"AT+CIPMUX=1\r\n");
    
     for(int i=0; i<=tam; i++)
        {
            TX1REG = cadena[i];
            while(TRMT==0);
        }
        __delay_ms(1000);
  
    tam = sprintf(cadena,"AT+CIPSERVER=1%c80\r\n",',');
    
     for(int i=0; i<=tam; i++)
        {
            TX1REG = cadena[i];
            while(TRMT==0);
        }
    
    __delay_ms(1000);
     
   
    
    while(1);
    
    return;
}
