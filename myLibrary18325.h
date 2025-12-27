/* 
 * File:   myLibraty18325.h
 * Author: h
 *
 * Created on 2020/1/15
 */

#ifndef MYLIBRARY18325_H
#define MYLIBRARY18325_H
        
#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>




//#define _XTAL_FREQ 8000000
#define _XTAL_FREQ 12000000
//USART---------------------------------------------------------------------
#define BAUDRATE 9600
#define SET_SPBRG_DATA  (((unsigned int)((_XTAL_FREQ/16)/BAUDRATE))-1)
#define SET_SPBRG_DATA1 (((unsigned int)((_XTAL_FREQ/4)/BAUDRATE))-1)

void USART_INIT(void);
//void putch(uint8_t byte);
//uint8_t getchar1(void);
//char getche(void);

struct {
    uint8_t length;
    uint8_t rxBuf[30];
    bool rxCompleted;
}usart;

//Timer0----------------------------------------------------------------------
#define T0_UP 1
#define T0_STOP 0
struct{
    bool up;
    uint8_t cnt[3];
}tm0;
void Timer0_INIT(void);

//Tiemr1----------------------------------------------------------------------
#define LH3

#ifdef  LH
#define TMR1_L 0x88//1ms
#define TMR1_H 0xFE//1ms
#endif

#ifdef  LH1
#define TMR1_L 0x44//0.5ms
#define TMR1_H 0xFF
#endif

#ifdef  LH2
#define TMR1_L 0xB4//0.2ms
#define TMR1_H 0xFF
#endif

#ifdef LH3
#define TMR1_L 0xDA//0.1ms
#define TMR1_H 0xFF
#endif

void Timer1_INIT(void);

//Interrupt On Change PORT---------------------------------------------------
void IOC_INIT(void);

//AD converter---------------------------------------------------------------
#define ADC_Sampling_Num 150
void ADC_INIT(void);
void ADC_Calibration(void);
uint32_t ADC_RMS(void);
struct{
    uint8_t Ch;
    uint16_t val;
    bool    readCompleted;
    uint16_t data[ADC_Sampling_Num+1];
    uint8_t data_index;
    uint16_t RMS;
    int32_t cal;
}ADC;

struct{
    uint8_t Ch;
    uint16_t val;
    int32_t voltage;
    bool readCompleted;
}ADC2;

struct{
    uint8_t Ch;
    uint16_t val;
    int32_t voltage;
    bool readCompleted;
}ADC3;

//Interrupt------------------------------------------------------------------
void Interrupt_START(void);

//innerEEPROM----------------------------------------------------------------
uint8_t eepromWrite(uint8_t add, uint8_t byte);
uint8_t eepromRead(uint8_t add);


#ifdef	__cplusplus
}
#endif

#endif