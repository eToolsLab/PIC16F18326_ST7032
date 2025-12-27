/* 
 * File:   I2C_MSSP1.h
 * Author: h
 *
 * 
 */

#ifndef I2C_REG_H
#define	I2C_REG_H
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
//#include "myLibrary18325.h"
#ifdef	__cplusplus
extern "C" {
#endif
//-----------------------------------------------
#define I2C1_Ack   0//SSP1CON2.ACKDT master:receive mode
#define I2C1_NoAck 1//SSP1CON2.ACKDT master:receive mode
void I2C1_Reg_Init(void);
bool I2C1_Start(void);
bool I2C1_ReStart(void);
bool I2C1_Stop(void);
bool I2C1_Tx_Buffer_Write(uint8_t _byte_data);
uint8_t I2C1_Rx_Buffer_Read(void);
bool I2C1_Wait_Ack(void);
bool I2C1_Send_Ack(uint8_t _Ack_Nack);
bool I2C1_CheckIdle(void);
bool I2C1_ClearSSPxIF(void);
//------------------------------------------------
//-----------------------------------------------
bool I2C1_b1Write(uint8_t _device_add, uint8_t _data);
uint8_t I2C1_b2Write(uint8_t _device_add, uint8_t _data1, uint8_t _data2);
uint8_t I2C1_DataBuffer[10];
bool I2C1_bnWrite(uint8_t _device_add, uint8_t *_data, uint8_t _len);
uint8_t I2C1_b1Read(uint8_t _device, uint8_t _data1);
uint16_t I2C1_b2Read(uint8_t _device_add, uint8_t _data1);
//-----------------------------------------------
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_REG_H */

