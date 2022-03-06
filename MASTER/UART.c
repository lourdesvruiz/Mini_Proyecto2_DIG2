#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "UART.h"

#define _XTAL_FREQ 8000000

void USART_Init(int frequency){
    
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1; //enable
    RCSTAbits.RX9 = 0;  //no comunicarme en 9 bits (modo de 8 bits)
    RCSTAbits.CREN = 1; //habilitamos la recepcion
    TXSTAbits.TXEN = 1; //habilitamos la transmision
    TXIF = 0;
    TXIE = 0;
    TXSTAbits.TX9 = 0; //no comunicarme en 9 bits
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    
   
    
    //Configuraciones para un baudrate de 9600 dependiendo de la frecuencia
    switch(frequency){
        case 4: 
            TXSTAbits.BRGH = 1;
            BAUDCTLbits.BRG16 = 0;
            SPBRG = 25;
            SPBRGH = 0;
            break;
        case 8:
            TXSTAbits.BRGH = 0;
            BAUDCTLbits.BRG16 = 0;
            SPBRG = 12;
            SPBRGH = 0;
            break;
    }
}

void give(char bit_cadena)
{
    //while(TXSTAbits.TRMT ==0);
    while (PIR1bits.TXIF ==0); //se espera a que el registro del transmisor este vacio
        TXREG = bit_cadena;    //le envia el dato a cada "bit" de la cadena. 
           
}

void strings(char *str)  //regresa un pointer hacia un caracter
{
    while (*str != '\0')  //siempre y cuando el pointer sea distinto a caracter nulo (Que no se haya terminado la cadena)
    {
        give(*str);       //la funcion de give ira tomando caracter por caracter
        str++;            //aumentando para ir mandando bit por bit 
                
    }
}
