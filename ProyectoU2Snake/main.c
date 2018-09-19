// 0.Sección de documentación 
// main.c
// Dispositivo: PIC16F18877
// Proyeccto Unidad 2, Microcontroladores
// Autores: Javier Camacho Pérez, Brandon Eduardo Palacios García, Ramces Mohamed 
// Fecha: Septiembre 17, 2018

// I/O List
//Inputs************************************************************************
// SW1 push button de lógica negativa conectado a RA0
// SW2 push button de lógica negativa conectado a RA1
// SW3 push button de lógica negativa conectado a RA2
// SW4 push button de lógica negativa conectado a RA3
//Outputs***********************************************************************
// Ánodos de matriz de LED's conectados al puerto D por medio de resistencias 
// Kátodos de matriz de LED's conectados al puerto B

// 1. Sección Directivas pre-proceso

// Incluir librerías
#include <xc.h>  //Esta es la librería principal del compilador en ella se encuentran todas las definiciones para el microcontrolador utilizado
#include <stdlib.h> //Librería estándar de C 
//#include <stdio.h>
#include <pic16f18877.h>
#include "config.h" //Archivo caberera donde se encuentran las configuraciones de los registros CONFIG1 a CONGIG5
 
// Definiciones
#define _XTAL_FREQ 32000000
//Los registros de cada bit del puerto A se definen como literales
#define C0 LATBbits.LATB0 
#define C1 LATBbits.LATB1 
#define C2 LATBbits.LATB2 
#define C3 LATBbits.LATB3 
#define C4 LATBbits.LATB4 
#define C5 LATBbits.LATB5 
#define C6 LATBbits.LATB6 
#define C7 LATBbits.LATB7 
//Los registros de los 4 primeros bits del puerto A se definen como literales
#define DERECHA PORTAbits.RA3
#define IZQUIERDA PORTAbits.RA0 
#define ABAJO PORTAbits.RA1
#define ARRIBA PORTAbits.RA2


// 2. Sección de declaraciones

//Variables Globales
int m[8][8]; //Matriz de visualizacíon
int n[64][2]; //Matriz del cuerpo de la serpiente
int x, y, cola, dir, k, vel; //Parámetros de la serpiente
int xx, yy; //Parámetros de la comida

//Prototipos de funciones
void barrer(void);
void pulsar(void);
void direccion(void);
void chocar(void);
void comida(void);
void avanzar(void);
void reiniciar(void);



// 3. Sección de subrutinas

// MAIN: obligatorio para un programa en C
void main(void) {

//Puertos
    
//Puerto B inicializado en cero y configurado como salida digital
    PORTB = 0;
    ANSELB = 0;
    TRISB = 0;
//Puerto D inicializado en cero y configurado como salida digital
    PORTD = 0;
    ANSELD = 0;
    TRISD = 0;
//Puerto A inicializado en cero y configurado como entrada digital
    PORTA = 0;
    ANSELA=0;
    TRISA = 0b11111111;
    WPUA=0b00001111; //Resistencias pull-up activadas en los puertos RA0, RA1, RA2 y RA3 
    
   srand(0); //Semilla para generar numeros random
    
    // Ciclo infinito 
    while(1){
        
        
    reiniciar(); //Se inicializan las variables 
    comida();    //Se genera la comida
         
        //Siempre que la serpiente este dentro de la matriz de vizualizacion el juego seguirá, si no el juego reinicia    
        while(x>=0 && x<8 && y>=0 && y<8){
           
            
             avanzar();
        //Ciclo para realizar el barrido de la matriz de LED's y checar si los botones son pulsados
             vel = cola;
             if(vel>32){
                 vel=32;
             }
             for(int i=0; i<(40-vel) ;i++){//La velocidad va aumentando segun el tamaño de la cola
            
             barrer();
             pulsar();
             
        }    
      
        direccion(); //Si algun botón fue pulsado la rirección de movimiento cambia
        
        chocar(); //Checa si la serpiente no se ha comido a si misma
       
        //Si la posición de la cabeza es igual a la de la comida, la serpiente crece una unidad y se genera mas comida 
         if(x == xx  &&  y == yy){
        
                     cola++;
                     comida();
            
                }
 
        }
        
    }
    return;
}


void comida(){
//Esta funcion genera la comida de la serpiente 
    
    //Se genera el numero random en un rango de -7 a 7 para las coordenadas y si el númuero que se genera es negativo se vuelve positivo
    xx = rand() % 7; 
    if(xx<0){
        xx *= (-1);
    }
    
    yy = rand() % 7;
    
    if(yy<0){
        yy *= (-1);
    }

    //Si la coordenada donde se genera la comida esta ocupada por el cuerpo esta se vuelve a generar 
    while(m[xx][yy]==1 ){
    
        
    xx = rand() % 7;
    
    if(xx<0){
        xx *= (-1);
    }
    
    yy = rand() % 7;
    
    if(yy<0){
        yy *= (-1);
    }
    
    
    }  
     
    //Se genera la comida en la matriz de visualización 
    m[xx][yy] = 1;
     
} 


void reiniciar(){
//Esta función inicializa los valores de las variables y arreglos 

//Valores iniciales de la coordenada de la cabeza de la serpiente
x=3;
y=2;
//Tamaño inicial de la serpiente
cola=1;
//Direccion inicial de movimiento de la serpiente
dir=3;
//Valor inicial de la constante de recorrimiento
k=0; 

//Ciclo para inicializar en cero la matriz de visualización
 for(int i=0; i<8; i++){
     
            for(int j=0; j<8; j++){
            
                m[i][j]=0;
                
            }
        }

//Ciclo para inicializar en cero la matriz de la serpiente
 for(int i=0; i<50; i++){
     
            for(int j=0; j<2; j++){
            
                n[i][j]=0;
                
            }
        }



}


void chocar(){
//Esta función revisa si la serpiente se come a si misma y en el caso de de verdadero se reinicia el juego
           
           for(int i=0; i<cola; i++){
            
             if(n[i][0] == x &&  n[i][1] == y){
            
                 reiniciar();  
                 comida();
                 
             }
        }
 
}


void avanzar(){
//Esta funcion avanza a la serpiente en la dirección proporcionada
    
    //Bora la ultima posicion del cuerpo de la serpiente para que se observe un avance
    m[ n[k][0] ] [ n[k][1] ] = 0;
    //La matriz del cuerpo toma los valores de la cabeza  
    n[k][0] = x;
    n[k][1] = y;
    //La matriz de visualización va tomando los valores proporcionados por la matriz del cuerpo 
    m[ n[k][0] ] [ n[k][1] ] = 1; 
    //La constante de recorrimoento aumenta
    k++;
    //Cuando la constante es igual al tamaño del cuerpo significa que la serpiente esta en su maximo tamaño y debe recorrerse
    if(k == cola){
        
        k = 0;
        
    }
     
}





 void barrer(){
 //Esta función lee los valores de la matriz de vizualizacíon y los imprime en la matriz de LED's por medio de un barrimiento    
      
 for(int i=0; i<8; i++){
     
      LATD = 0b00000001 << (i);  //Registro que activa las filas, una a la vez
             
      //Cuando en la matriz de visualización existe un uno se checa en que posicion de la columna esta y se activa el bit correspondiente
            for(int j=0; j<8; j++){
            
                if(m[i][j]==1){
                    
                    
                    if(j==0){
                        C0=0;
                    }
                    
                    if(j==1){
                        C1=0;
                    }
                    
                    if(j==2){
                        C2=0;
                    }
                    
                    if(j==3){
                        C3=0;
                    }
                    
                    if(j==4){
                        C4=0;
                    }
                    
                    if(j==5){
                        C5=0;
                    }
                    
                    if(j==6){
                        C6=0;
                    }
                    
                    if(j==7){
                        C7=0;
                    }
                    
                    
                }
              
             } 
             
              __delay_ms(2);  //Es el tiempo que permanecen activados los LED's de cada fila durante el barrimiento
           
             LATB = 0b11111111; //Se desavtiva el registro de las columnas para activar la fila siguiente
            
            }
         
 }
 
 void pulsar(){
 //Esta función cambia la rirección de movimiento de la serpiente de acuerdo al boton que se pulse
 //Si la direccion deseada es opuesta a la actual mo se permite el cambio de dirección
     
        if(ARRIBA==0 && dir!=2){
            dir=1; 
        }
        
        if(ABAJO==0 && dir!=1){
            dir=2; 
        }
        
        if(DERECHA==0 && dir!=4){
            dir=3; 
        }
        
        if(IZQUIERDA==0 && dir!=3){
            dir=4;
        }
        
        
 }
 
 
 void direccion(){
 //Esta función mueve la coordenada de la cabaeza en la dirección proporcionada 
     
        if(dir==1){
            x--;
        }
        
        if(dir==2){
            x++;
        }
        
        if(dir==3){
            y++;
        }
        
        if(dir==4){
            y--;
        }

 }
