// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Práctica 3.2
// Autores: Brandon Eduardo Palacios García, Javier Camacho Pérez, Ramces Hernández Romero
// Fecha: Octubre 1, 2018

// I/O List
//Inputs************************************************************************
// Filas teclado matricial de botones en C0, C2, C3, C4
// Columnas teclado matricial de botones en D4, D5, D6, D7 
//Outputs*************************************************************
//Pines pantalla LCD 16x2 RA-RA5

// 1. Sección Directivas pre-proceso
// Incluir librerías
// Definiciones
// Macros
#include <xc.h>  //Esta es la librería principal del compilador
#include "config.h" //Configuraciones del microcontrolador
#define _XTAL_FREQ 32000000 //Declaracion de la frecuencia del oscilador
#include "LCD.h" // Libreria LCD
#include <stdio.h> //Libreria estandar de entradas y salidas en C
#include <stdbool.h> //Libreria para usar variables boleanas
#include <pic16f18877.h> //Libreria del microcontrolador
// 2. Sección de declaraciones
//   Variables Globales

int k; //Posición para imprimir en la LCD
int signo; //Varable que guarda el signo 
int v[3]; //Vector que guarda los digitos del número
int m,n; //posiciónes de la matriz
int fila,columna;//Variables que guardan los regitrsos para las filas y columnas

double num1,num2,resultado; //Variables que guardan los numeros ingresados y el resultado de la operación 

bool selec=true,pulsar=false,bloquear=true; //Variables de restricción 

unsigned char buffer1[16]; //Variable para la LCD
unsigned char x; //Variable de la tecla actual
unsigned char w; //Variable para recorrer bits puerto D

char teclado[4][4] = {{'7', '8', '9', '/'},
                      {'4', '5', '6', '*'},
                      {'1', '2', '3', '-'},
                      {'C', '0', '=', '+'}};  //Matriz para guardar los valores del teclado 

//   Prototipos de funciones



// 3. Sección de subrutinas
// MAIN: obligatorio para un programa en C
void main(void) {
    //Configuracion de puertos
    PORTA=0;
    PORTC=0;
    PORTD=0;
    
    ANSELA=0;
    ANSELC=0;
    ANSELD=0;
    
    TRISA=0;
    TRISC=0b00001111;
    TRISD=0;

    
    WPUC=0b00001111;
    WPUEbits.WPUE3=1;
    
    //Configuracion LCD
    Lcd_Init(); //Inicializar LCD
    Lcd_Cmd(LCD_CLEAR); //LimpiarLCD
    Lcd_Cmd(LCD_CURSOR_OFF); //Apagar cursor
    
    //Configuracion de interrupcion IOC
    IOCCN=0b00001111; //Activa la interrupcion por flanco de bajada 
    PIR0bits.IOCIF=0; //Limpia bandera general de la interrupcion
    IOCCF=0b00000000; //Limpia las banderas del puerto C
    PIE0bits.IOCIE=1; //Activa la interrupcion IOC
    INTCONbits.GIE=1; //Activa la interrupcion global 
  
    while(1)
    {
        //Ciclo para activar una columna a la vez
        for(int i=0; i<4; i++)
        {
            w = 0b00010000<<i;
            LATD = ~w;
            __delay_ms(10);              
        }
        
     while(pulsar) //Solo entra cuando se ha pulsado una tecla 
    { 
  
    if((x=='0' || x=='1' || x=='2' || x=='3' || x=='4' || x=='5' || x=='6' || x=='7' || x=='8' || x=='9') && bloquear )//Si se ha presionado un numero y si la operacion no ha finalizado
    {
        if(k>3)
        {
            k=3; //K se limita para utilizar solo 3 digitos 
        }
        if(selec) //Si se asigna el primer numero 
        {    
          v[k] = x -48; //Convierte la variable char a numero 
          if(k==0){num1= v[0];} //Asigna la unidad
          if(k==1){num1= v[0]*10 +v[1];} // Asigna unidad y decena
          if(k==2){num1= v[0]*100 + v[1]*10 +v[2];} //Asigna unidad decena y centena 
          sprintf(buffer1,"%.lf",num1); //Imprime el valor 
          Lcd_Out2(1, 0, buffer1);
          k++; //K aumenta para el siguiente digito 
        }
        
        else //Si se activa el segundo numero 
        {
          v[k] = x -48; 
          if(k==0){num2= v[0];}
          if(k==1){num2= v[0]*10 +v[1];}
          if(k==2){num2= v[0]*100 + v[1]*10 +v[2];}
          sprintf(buffer1,"%.lf",num2);
          Lcd_Out2(2, 0, buffer1);
          k++;
        }
      
    
    }
       
       
    
    if((x=='+' || x=='-' || x=='*' || x=='/') && selec && bloquear) //Si se presiona un signo, solo despues de ingresar el primer numero y si la operacion no termina
    {
        if(k>3)
        {
            k=3;
        }
          
        if(x=='+')
        {
            sprintf(buffer1,"%c",x);
            Lcd_Out2(1, k, buffer1);
            signo=1; // Asigna la operacion 
            selec=false; //si se presiona un signo salta a la fila 2
            k=0;
        }
        
        if(x=='-')
        {
            sprintf(buffer1,"%c",x);
            Lcd_Out2(1, k, buffer1);
            signo=2;
            selec=false;
            k=0;
        }
        
         if(x=='*')
        {
            sprintf(buffer1,"%c",x);
            Lcd_Out2(1, k, buffer1);
            signo=3;
            selec=false;
            k=0;
        }
        
        if(x=='/')
        {
            sprintf(buffer1,"%c",x);
            Lcd_Out2(1, k, buffer1);
            signo=4;
            selec=false;
            k=0;
        }
        
     
    
    }
    
    
     if(x=='=') //Si se presiona el digno igual
        { 
            switch (signo) //Checa cual signo se activo
            {
                case 1: //Suma
                  resultado=num1+num2;
                  Lcd_Cmd(LCD_CLEAR);	
                  sprintf(buffer1,"%.lf",resultado);
                  Lcd_Out2(1, 0, buffer1);       
                  bloquear=false;
                break;
              
                case 2: //Resta
                  resultado=num1-num2;
                  Lcd_Cmd(LCD_CLEAR);
                  sprintf(buffer1,"%.lf",resultado);
                  Lcd_Out2(1, 0, buffer1);
                  bloquear=false;
                break;
                
                case 3: //Multiplicacion
                  resultado=num1*num2;
                  Lcd_Cmd(LCD_CLEAR);
                  sprintf(buffer1,"%.lf",resultado);
                  Lcd_Out2(1, 0, buffer1); 
                  bloquear=false;
                break;
                
                case 4: //Division
                  resultado=num1/num2;
                  Lcd_Cmd(LCD_CLEAR);
                  sprintf(buffer1,"%.3lf",resultado);
                  Lcd_Out2(1, 0, buffer1);
                  bloquear=false;
                break;
            }
        }
    
            
        if(x=='C') //Limpia todos los valores de las variables
        {
           Lcd_Cmd(LCD_CLEAR);
           k=0;
           num1=0;
           num2=0;
           resultado=0;
           signo=0;
           v[0]=0;
           v[1]=0;
           v[2]=0;
           selec=true;
           bloquear=true;
           x=0;        
        
        }
    
    
      pulsar=false; //Cambia para que solo re realice una vez
    
    
    }
        
    }
     return;
}


void __interrupt() ISR (void)
{
    pulsar=true; //Pulsar verdadero para que entre al ciclo while
    __delay_ms(250);
    fila=IOCCF; 
    columna=255-PORTD;
    
    switch (fila)//Checa que bit activo la interrupcion 
    {
        case 1:
            m=0;
        break;
          
        case 2:
            m=1;
        break;
        
        case 4:
            m=2;
        break;
        
        case 8:
            m=3;
            break;
    }
     
    switch (columna) //Checa que bit activo el boton 
    {
        case 16:
            n=0;
        break;
          
        case 32:
            n=1;
        break;
        
        case 64:
            n=2;
        break;
        
        case 128:
            n=3;
            break;
    }
            
     x = teclado[m][n]; //Asigna la fila y columna a la matriz para leer la tecla 
 
    IOCCF=0; //Limpia las banderas de la interrupcion IOC 
    PIR0bits.IOCIF=0; //Limpia la bandera global de la interrupcion 
       
}
