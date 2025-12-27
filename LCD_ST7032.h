/* 
 * File:   LCD_ST7032.h
 * Author: h
 *
 * Created on 2025/12/27, 15:49
 */

#ifndef LCD_ST7032_H
#define	LCD_ST7032_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#define LCD_ST7032_deviceAdd 0x7C 
    
#define High 1
#define Low  0    

#define CM 0    
#define DT 1
    
typedef union {
	uint8_t byt;
	struct{
		uint8_t D0:1;//LSB
		uint8_t D1:1;
		uint8_t D2:1;
		uint8_t D3:1;
		uint8_t D4:1;
		uint8_t D5:1;
		uint8_t RS:1;
		uint8_t C0:1;//MSB
	};
}_ControlByte; 
    
extern _ControlByte CB; 

extern void LCD_ST7032_Init(uint8_t deviceAdd);   
extern void LCD_ST7032_Send(uint8_t deviceAdd,uint8_t mode,uint8_t data);   
extern void LCD_ST7032_WriteData(uint8_t deviceAdd,uint8_t data);
extern void LCD_ST7032_Command(uint8_t deviceAdd,uint8_t command);
extern void LCD_ST7032_Printf(uint8_t deviceAdd, uint8_t *string, uint8_t length,uint8_t DDRAM_ADD);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_ST7032_H */

