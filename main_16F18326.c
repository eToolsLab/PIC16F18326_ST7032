// PIC16F18326 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1  // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; I/O or oscillator function on OSC2)
#pragma config CSWEN = ON      // Clock Switch Enable bit (The NOSC and NDIV bits cannot be changed by user software)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config MCLRE = ON     // Master Clear Enable bit (MCLR/VPP pin function is digital input; MCLR internally disabled; Weak pull-up under control of port pin's WPU control bit.)
#pragma config PWRTE = OFF       // Power-up Timer Enable bit (PWRT enabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable bits (WDT disabled; SWDTEN is ignored)
#pragma config LPBOREN = OFF    // Low-power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled)
#pragma config BORV = LOW       // Brown-out Reset Voltage selection bit (Brown-out voltage (Vbor) set to 2.45V)
#pragma config PPS1WAY = ON     // PPSLOCK bit One-Way Set Enable bit (The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a Reset)
#pragma config DEBUG = ON      // Debugger enable bit (Background debugger disabled)

// CONFIG3
#pragma config WRT = OFF        // User NVM self-write protection bits (Write protection off)
#pragma config LVP = ON        // Low Voltage Programming Enable bit (High Voltage on MCLR/VPP must be used for programming.)

// CONFIG4
#pragma config CP = OFF         // User NVM Program Memory Code Protection bit (User NVM code protection disabled)
#pragma config CPD = OFF        // Data NVM Memory Code Protection bit (Data NVM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "myLibrary18325.h"
#include "LCD_ST7032.h"
#include "I2C_MSSP1.h"

#define LED2 RA2


void Port_init(void);


void __interrupt()isr()
{  
    if(PIR0bits.TMR0IF)
    {     
        tm0.cnt[0]++;
       
               
        if(tm0.cnt[0]==5)
        {
            tm0.up=T0_UP;
            
            tm0.cnt[0]=0;
        }else
        {
            TMR0L=0x61;//100ms
            PIR0bits.TMR0IF=0;
        } 
    }      
}


int main(int argc, char** argv)
{
    uint16_t counter;
    uint8_t length;
    uint8_t txt[15];
    //Oscillator
    OSCCON1bits.NOSC=0b110;
    OSCCON1bits.NDIV=0b000;
    OSCFRQbits.HFFRQ=0b101; //100:8MHz 101:12Mhz
    __delay_ms(10);
    //initial--------------------------------------
    Port_init();
    I2C1_Reg_Init();
    __delay_ms(50);
    LCD_ST7032_Init(LCD_ST7032_deviceAdd);
    LCD_ST7032_Command(LCD_ST7032_deviceAdd,0x80);
    LCD_ST7032_WriteData(LCD_ST7032_deviceAdd,0x41);
    LCD_ST7032_Printf(LCD_ST7032_deviceAdd,"AQM1602A",length,0x80);
    Timer0_INIT();
            
    Interrupt_START();
    
    counter=0; 
    while(1)
    {
             
       if(tm0.up==T0_UP)
       {
           LATAbits.LATA5=~LATAbits.LATA5;
            INTCONbits.GIE=0;
            INTCONbits.PEIE=0;
            tm0.up=T0_STOP;
            length = sprintf(txt,"%07d",counter++);
            LCD_ST7032_Printf(LCD_ST7032_deviceAdd,txt,length,0xC0);
            PIE1bits.TMR1IE=1;
            INTCONbits.GIE=1;
       }
          
    }
    return (EXIT_SUCCESS);
}

void Port_init(void)
{
    //Port init--------- 
    //PORTA initialize
    TRISA = 0x08;   //RA3:MCLR 
    ODCONA = 0x00;  //When anODCONA bit is set, the corresponding port output becomes an open-drain driver capable of sinking current only.
    ANSELA = 0x00;
    WPUA =0x00;
    PORTA=0x00;

    //PORTC initialize
    TRISC = 0x03;//RC0;SCL RC1:SDA
    ODCONC =0x00;
    ANSELC=0x00;
    WPUC=0x00;
    PORTC=0x00;
    LATC =0x00;
    
//PPS init-----------------------    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0;
  
    //I2C RC0:SCL1 RC1:SDA1
    SSP1CLKPPS=0x10;
    SSP1DATPPS=0x11;
    RC0PPS=0x18;
    RC1PPS=0x19;
    TRISC0=1;
    TRISC1=1;
    
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 1;
//PPS re-Locked---------------------
}


