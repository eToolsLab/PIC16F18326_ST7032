
#include "myLibrary18325.h"
#include "LCD_ST7032.h"
#include "I2C_MSSP1.h"

_ControlByte CB;

/**
 * LCD_ST7032_Init
 * @param deviceAdd クライアントアドレス
 */
void LCD_ST7032_Init(uint8_t deviceAdd)
{
    __delay_ms(50);
    //Function set
    LCD_ST7032_Command(deviceAdd,0x38);
    __delay_us(50);
    //Function set
    LCD_ST7032_Command(deviceAdd,0x39);
    __delay_us(50);
    //Internal OSC frequency
    LCD_ST7032_Command(deviceAdd,0x17);
    __delay_us(50);
    //Contrast set
    LCD_ST7032_Command(deviceAdd,0x73);
    __delay_us(50);
    //Power/ICON/Contrast control
    LCD_ST7032_Command(deviceAdd,0x56);
    __delay_us(50);
    //Folower control
    LCD_ST7032_Command(deviceAdd,0x6C);
    __delay_ms(300);
    //Function set
    LCD_ST7032_Command(deviceAdd,0x38);
    __delay_us(50);
    //Clear Display
    LCD_ST7032_Command(deviceAdd,0x01);
    __delay_ms(1);
    //Display ON/OFF control
    LCD_ST7032_Command(deviceAdd,0x0C);
    __delay_us(50);
}

/**
 * LCD_ST7032_WriteData
 * @param deviceAdd クライアントアドレス
 * @param data データ
 */
void LCD_ST7032_WriteData(uint8_t deviceAdd,uint8_t data)
{
    CB.byt = 0x00;
    CB.C0 = 0;
    CB.RS = DT;//H:Data write L:Instruction write
    I2C1_b2Write(deviceAdd,CB.byt,data);
    __delay_us(20);
}

/**
 * LCD_ST7032_Command
 * @param deviceAddクライアントアドレス
 * @param commandコマンド
 */
void LCD_ST7032_Command(uint8_t deviceAdd,uint8_t command)
{
    CB.byt = 0x00;
    CB.C0 = 0;
    CB.RS = CM;//H:Data write L:Instruction write
    I2C1_b2Write(deviceAdd,CB.byt,command);
    __delay_us(20);
}

/**
 * LCD_ST7032_Printf
 * @param deviceAddクライアントアドレス
 * @param string表示文字列
 * @param length表示文字列長
 * @param DDRAM_ADD表示位置アドレス
 */
void LCD_ST7032_Printf(uint8_t deviceAdd, uint8_t *string, uint8_t length,uint8_t DDRAM_ADD)
{
    LCD_ST7032_Command(deviceAdd,DDRAM_ADD);
    do{
        LCD_ST7032_WriteData(deviceAdd,*string);
        string++;
    }while(--length);
}

