
#include <xc.h>
#include "I2C.h"
#include "MPU.h"
#include "LCD.h"
#include "UART.h"  // for debugging serial terminal
#include <stdio.h>

//-------------[ MPU6050 Routines ]------------------
//---------------------------------------------------
int GX,GY,GZ,Gx,Gy,Gz;
int X, Y;
int flag_x, flag_y;

//Funciones para obtener el valor del giroscopio en ascii
int conversion_x();
int conversion_y();
unsigned char MPU_conversion(uint8_t  conv);

//Función de inicialización de comunicacion I2C con sensor
void MPU6050_Init()
{
  // Power-Up Delay & I2C_Init
  __delay_ms(100);
  //I2C_Master_Init(100000);
 
  // Setting The Sample (Data) Rate
  I2C_Start(0xD0);
  MPU_Master_Write(SMPLRT_DIV);
  MPU_Master_Write(0x07);
  I2C_Master_Stop();
 
  // Setting The Clock Source
  I2C_Start(0xD0);
  MPU_Master_Write(PWR_MGMT_1);
  MPU_Master_Write(0x01);
  I2C_Master_Stop();
 
  // Configure The DLPF
  I2C_Start(0xD0);
  MPU_Master_Write(CONFIG);
  MPU_Master_Write(0x00);
  I2C_Master_Stop();
 
  // Configure The ACCEL (FSR= +-2g)
  I2C_Start(0xD0);
  MPU_Master_Write(ACCEL_CONFIG);
  MPU_Master_Write(0x00);
  I2C_Master_Stop();

  // Configure The GYRO (FSR= +-2000d/s)
  I2C_Start(0xD0);
  MPU_Master_Write(GYRO_CONFIG);
  MPU_Master_Write(0x18);
  I2C_Master_Stop();
 
  // Enable Data Ready Interrupts
  I2C_Start(0xD0);
  MPU_Master_Write(INT_ENABLE);
  MPU_Master_Write(0x01);
  I2C_Master_Stop();
}

void MPU6050_Read()
{
  //char buffer[40];
  
  // Prepare For Reading, Starting From ACCEL_XOUT_H
  I2C_Start(0xD0);
  MPU_Master_Write(ACCEL_XOUT_H);
  I2C_Master_Stop();
  I2C_Start(0xD1);
 
  //Medición de rotación angular 
  
  Gx = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gy = ((int)I2C_Read(0)<<8) | (int)I2C_Read(0);
  Gz = ((int)I2C_Read(0)<<8) | (int)I2C_Read(1);
  I2C_Master_Stop();
  
  // Conversion a grados por segundo 
  GX = ((Gx/131)*100);  //se multiplica por 100 para obtener un valor más elevado
  GY = ((Gy/131)*100);
  
  // Banderas para valores negativos 
  if (GX < 0){
      X = GX*(-1);
      flag_x = 1;
  }
  
  else{
      //X = Gx;
      flag_x = 0;
  }
  
  if (GY < 0){
      Y = GY*(-1);
      flag_y = 1;
  }
  
  else{
      //X = Gx;
      flag_y = 0;
  }
 
  Lcd_Set_Cursor(1,1);
  Lcd_Write_String("Gx:");
  Lcd_Write_String(conversion_x());
  strings("Gx:");
  strings(conversion_x());
  strings(" ");
  
  Lcd_Set_Cursor(2,1);
  Lcd_Write_String("Gy:");
  Lcd_Write_String(conversion_y());
  strings("Gy:");
  strings(conversion_y());
  strings(" ");
  strings("\n");
 
}

unsigned char MPU_conversion(uint8_t  conv){
    switch(conv){
        case 0: 
            return 48;
            break;
        case 1: 
            return 49;
            break;
        case 2: 
            return 50;
            break;
        case 3: 
            return 51;
            break;
        case 4: 
            return 52;
            break;
        case 5: 
            return 53;
            break;
        case 6: 
            return 54;
            break;
        case 7: 
            return 55;
            break;
        case 8: 
            return 56;
            break;
        case 9: 
            return 57;
            break;
       
    }
}

int conversion_x(){
    uint8_t buffer[6];
    if (flag_x == 1){
      //buffer[0] = 32;  //espacio
      buffer[1] = 45;  //signo negativo
      buffer[2] = MPU_conversion((X/10000));
      buffer[3] = MPU_conversion(((X/1000)%10));
      buffer[4] = MPU_conversion(((X/100)%10));
      buffer[5] = MPU_conversion(((X/10)%10));
      buffer[6] = MPU_conversion(((X/1)%10));
      //return buffer;
  }
  
    else if (flag_x == 0){
      //buffer[0] = 32;
      buffer[1] = MPU_conversion((GX/10000));
      buffer[2] = MPU_conversion(((GX/1000)%10));
      buffer[3] = MPU_conversion(((GX/100)%10));
      buffer[4] = MPU_conversion(((GX/10)%10));
      buffer[5] = MPU_conversion(((GX/1)%10));
      //return buffer;
  }
    return buffer;
}

int conversion_y(){
    uint8_t buffer[6];
    if (flag_y == 1){
      //buffer[0] = 32;
      buffer[1] = 45;
      buffer[2] = MPU_conversion((Y/10000));
      buffer[3] = MPU_conversion(((Y/1000)%10));
      buffer[4] = MPU_conversion(((Y/100)%10));
      buffer[5] = MPU_conversion(((Y/10)%10));
      buffer[6] = MPU_conversion(((Y/1)%10));
      //return buffer;
  }
  
    else if (flag_y == 0){
      //buffer[0] = 32;
      buffer[1] = MPU_conversion((GY/10000));
      buffer[2] = MPU_conversion(((GY/1000)%10));
      buffer[3] = MPU_conversion(((GY/100)%10));
      buffer[4] = MPU_conversion(((GY/10)%10));
      buffer[5] = MPU_conversion(((GY/1)%10));
      //return buffer;
  }
    return buffer;
}
