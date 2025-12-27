

#include "myLibrary18325.h"
#include "I2C_MSSP1.h"

void I2C1_Reg_Init(void)
{
    SSP1STAT=0xC0;
    SSP1CON1=0x28;
    SSP1CON3=0x00;
    //SSP1ADD=0x50;//Fosc=16Mhz Fclock=50kHz
    //SSP1ADD=0x27;//Fosc=16MHz Fclock=100kHz
    SSP1ADD=0x1D;//Fosc=12MHz Fclock=100kHz
    PIE1bits.SSP1IE=0;
    PIE1bits.BCL1IE=1;
}

//--------------------------------------------
bool I2C1_Start(void)
{
    if(!I2C1_CheckIdle()) return false;
    SSP1CON2bits.SEN=1;
    if(!I2C1_ClearSSPxIF())return false;
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return true;
}

bool I2C1_ReStart(void)
{
     //restart
    if(!I2C1_CheckIdle()) return false;
    SSP1CON2bits.RSEN=1;
     NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return true;
    
}

bool I2C1_Stop(void)
{
     if(!I2C1_CheckIdle()) return false;
    SSP1CON2bits.PEN=1;
    if(!I2C1_ClearSSPxIF())return false;
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return true;
}

bool I2C1_Tx_Buffer_Write(uint8_t _byte_data)
{
    if(!I2C1_CheckIdle()) return false;
    SSP1BUF=_byte_data;
    if(!I2C1_ClearSSPxIF())return false;
    NOP();
    NOP();
    NOP();
    NOP();
    NOP();
    return true;
}

uint8_t I2C1_Rx_Buffer_Read(void)
{
    uint8_t ret, cnt;
    ret=0xFF;
    cnt=0;
    if(!I2C1_CheckIdle()) return false;
    SSP1CON2bits.RCEN=1;
    while(!SSP1STATbits.BF){
        __delay_us(3);
        cnt++;
        if(cnt==100)return false;
    }
    if(!I2C1_CheckIdle()) return false;
    ret = SSP1BUF;
    if(!I2C1_ClearSSPxIF())return false;
    return ret;
}

bool I2C1_Wait_Ack(void)
{
    //while(SSP1CON2bits.ACKSTAT);
    if(SSP1CON2bits.ACKSTAT)
    {
        return false;//error
        
    }else
    {
        return true;//success
    }
}

bool I2C1_Send_Ack(uint8_t _Ack_Nack)
{
    if(!I2C1_CheckIdle()) return false;
    SSP1CON2bits.ACKDT=_Ack_Nack;
    SSP1CON2bits.ACKEN=1;   
    if(!I2C1_ClearSSPxIF())return false;
    return true;
}

bool I2C1_CheckIdle(void)
{
    uint8_t cnt=0;
    while(SSP1STATbits.R_nW || SSP1CON2 & 0x1F)
    {
        __delay_us(2);
        cnt++;
        if(cnt==100) 
            return false;
    };//MSSP1 is idle mode
    return true;
}

bool I2C1_ClearSSPxIF(void)
{
    uint8_t cnt=0;
    while(!PIR1bits.SSP1IF)
    {
        __delay_us(2);
        cnt++;
        if(cnt==100)
            return false;
    };
    PIR1bits.SSP1IF=0;
    return true;
}
//-----------------------------------------------------------------

bool I2C1_b1Write(uint8_t _device_add, uint8_t _data)
{  
    //start condition
    I2C1_Start();
    //send device address
    I2C1_Tx_Buffer_Write(_device_add);
    if(!I2C1_Wait_Ack())
        return false;
    //send data
    I2C1_Tx_Buffer_Write(_data);
    if(!I2C1_Wait_Ack())
        return false;
    //Stop condition
    I2C1_Stop();
    return true;
}

uint8_t I2C1_b2Write(uint8_t _device_add, uint8_t _data1, uint8_t _data2)
{
    //start condition
    I2C1_Start();
    //send device address
    I2C1_Tx_Buffer_Write(_device_add);
    if(!I2C1_Wait_Ack())
    {
        return 0xF1;
        //return false;
    }
    //send data
    I2C1_Tx_Buffer_Write(_data1);
    if(!I2C1_Wait_Ack())
    {    
        return 0xF2;
        //return false;
    }
        //send data
    I2C1_Tx_Buffer_Write(_data2);
    if(!I2C1_Wait_Ack())
    {
        return 0xF3;
        //return false;
    }
    //Stop condition
    I2C1_Stop();
    return true;
}
bool I2C1_bnWrite(uint8_t _device_add, uint8_t *_data, uint8_t _len)
{
    //start condition
    I2C1_Start();
    //send device address
    I2C1_Tx_Buffer_Write(_device_add);
     if(!I2C1_Wait_Ack())
        return false;
    //send data
    do{
        I2C1_Tx_Buffer_Write(*_data);
        if(!I2C1_Wait_Ack())
        return false;
        _data++;
    }while(--_len);
    //Stop condition
    I2C1_Stop();
    return true;
}

uint8_t I2C1_b1Read(uint8_t _device_add, uint8_t _data1)
{
    uint8_t ret;
    //Start Condition
    I2C1_Start();
     //send device address
    I2C1_Tx_Buffer_Write(_device_add);
     if(!I2C1_Wait_Ack())
        return false;
    //send data
    I2C1_Tx_Buffer_Write(_data1);
     if(!I2C1_Wait_Ack())
        return false;
    //restart
    I2C1_ReStart();
      //send device address
    I2C1_Tx_Buffer_Write(_device_add|0x01);
     if(!I2C1_Wait_Ack())
        return false;
    //recieve data
    ret=I2C1_Rx_Buffer_Read();
    //send Nack
    I2C1_Send_Ack(I2C1_NoAck);
    //Stop Condition
    I2C1_Stop();
    return ret;
}

uint16_t I2C1_b2Read(uint8_t _device_add, uint8_t _data1)
{
    uint16_t ret;
    //Start Condition
    I2C1_Start();
     //send device address
    I2C1_Tx_Buffer_Write(_device_add);
     if(!I2C1_Wait_Ack())
        return false;
    //send data
    I2C1_Tx_Buffer_Write(_data1);
     if(!I2C1_Wait_Ack())
        return false;
    //restart
    I2C1_ReStart();
    //send device address
    I2C1_Tx_Buffer_Write(_device_add|0x01);
     if(!I2C1_Wait_Ack())
        return false;
    //recieve data
    ret=I2C1_Rx_Buffer_Read();
    ret<<=8;
    //send Ack
    I2C1_Send_Ack(I2C1_Ack);
    //recieve data
    ret|=I2C1_Rx_Buffer_Read();
    //send Nack
    I2C1_Send_Ack(I2C1_NoAck);
    //Stop Condition
    I2C1_Stop();
    return ret;
}