/*
 * File:   main.c
 * Author: Lourdes Ruiz
 * Ejemplo de uso de I2C Master
 * Created on 17 de febrero de 2020, 10:32 AM
 */
//*****************************************************************************
// Palabra de configuraci?n
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//*****************************************************************************
// Definici?n e importaci?n de librer?as
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "LCD.h"
#include "UART.h"
#include "MPU.h"
#include <xc.h>
//*****************************************************************************
// Definici?n de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000

//*****************************************************************************
// Definici?n de funciones para que se puedan colocar despu?s del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************

/*
 * Variables 
 */

uint8_t contador;
uint8_t centena, decena, unidad;
uint8_t z;
uint8_t dato;
uint8_t semaforo;
char ESP32;

/*
 * Prototipos de funciones
 */
void setup(void);

/*
 * Interrupcion 
 */

void __interrupt() isr (void)
{
   
    //Se guarda el valor que se ingresa por medio del slider de Adafruit
    if(RCIF){
        ESP32 = RCREG; 
        RCIF = 0;
        
    }
    
}

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    
    MPU6050_Init();
    
    TRISD = 0x00;
    Lcd_Clear();
    Lcd_Set_Cursor(1,11);                //settear el cursor
    Lcd_Write_String("Sem:");
    
    while(1){
        
        //Lectura del sensor
        MPU6050_Read();
        __delay_ms(50);
        
        //Se manda el valor actual de contador a PIC esclavo
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(contador);
        I2C_Master_Stop();
        __delay_ms(200);
        
       
        Lcd_Set_Cursor(2,11);
        
        //Color de semaforo dependiendo del slider de Adafruit en el LCD
        if (ESP32 == 1){
            contador = 0;
            Lcd_Write_String(" Verde");
        }
        else if (ESP32 == 2){
            contador = 1;
            Lcd_Write_String("Yellow");
        }
        else if (ESP32 == 3){
            contador = 2;
            Lcd_Write_String("  Rojo");
        }
        
        
      
         
    }
    return;
}

void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTA = 0;
    
     //Configuracion de oscilador
    OSCCONbits.IRCF = 0b0111; //8MHz
    OSCCONbits.SCS = 1; //ocsilador interno
    
    
    USART_Init(8);
    
    Lcd_Init();
    
    I2C_Master_Init(100000);        // Inicializar Comunicacion I2C
    
    //Configuracion de las interrupciones
    PIR1bits.RCIF = 0; //apagar la bandera de UART
    PIE1bits.RCIE = 1; //habilitar interrupcion 
    
    INTCONbits.PEIE = 1; //interrupciones perifericas
    INTCONbits.GIE  = 1;
}
