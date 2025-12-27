/*
 *File: myLibrary.c 
 *CPU: PIC16F18325
 * 19.12.26
 */
#include "myLibrary18325.h"

/*-----------------------------------------
 *USART
 *-----------------------------------------*/
void USART_INIT(void)
{
    uint8_t buf;
    unsigned int brg;
    //SPBRG value set
    brg = SET_SPBRG_DATA1;
    SP1BRGL =(unsigned char) brg;
    SP1BRGH =(unsigned char) (brg>>8); 
    //TX-------------------------------------
    TX1REG=0x00;
    //TXSTA SYNC:1 BRGH:1 TXEN:1 TX9:0
    buf =TX1STA & 0x83;
    TX1STA = buf | 0x24;
    //RX-------------------------------------
    //RCSTA  SPEN:1 RX9:0 CREN:1
    buf =RC1STA & 0x2F;
    RC1STA =buf | 0x90;
    //BAUR RATE CONTROL REGISTER
    BAUD1CON = 0x08;
    //if interrupt is desired, TXIE set(PIE1)
    PIE1bits.TXIE=0;//disable
    //if interrupts are desired, RCIE set(PIE1)
    PIE1bits.RCIE=1;//enable
    
    usart.length=0;
    usart.rxCompleted=false;
}

void putch(char byte)
{
    while(!PIR1bits.TXIF)continue;
    TX1REG=byte;
}

int getch(void)
{
   // char ch;
    while(!PIR1bits.RCIF){}
    return RC1REG; 
}

/*---------------------------------------------------
 Timer0
 ---------------------------------------------------*/
void Timer0_INIT(void)
{
    TMR0L=0x61;     //Fosc:12Mhz 100ms counter value Low byte
    TMR0H=0xDB;     //Fosc:12Mhz 100ms counter value High byte
    T0CON0=0x90;    //T0EN:1 16bits counter:1     
    T0CON1=0x45;    //ClockSource:Fosc/4 PSC:1/32
    PIE0bits.TMR0IE=1;
    tm0.up=T0_STOP;
    tm0.cnt[0]=0x00;
    tm0.cnt[1]=0x00;
}
/*---------------------------------------------------
 Timer1
 ---------------------------------------------------*/

void Timer1_INIT(void)
{
    T1CONbits.TMR1CS=0b00;  //Timer Clock Sorce :Fosc/4
    T1CONbits.T1CKPS=0b11;  //Prescale:1:8
    T1CONbits.T1SOSC=1;
    T1CONbits.T1SYNC=1;     //ignored. CS=0x00;
    T1CONbits.TMR1ON=1;     //Timer1 Enable.
    T1GCON=0x00;
    TMR1L=TMR1_L; //0.5ms 0xFF44
    TMR1H=TMR1_H;
    PIE1bits.TMR1IE=1;
    PIR1bits.TMR1IF=0;
}

/*------------------------------------------------
 * Interrupt On Change
 ---------------------------------------------------*/
void IOC_INIT(void)
{
    uint8_t buf;
     //IOC
    IOCAP=0x20;//PORTA Positive edge RA5:IOC
    buf=TRISA;
    TRISA=buf|0x20;
    PIE0bits.IOCIE=1;
}

/*---------------------------------------------------
 AD converter
 -------------------------------------------------------*/
void ADC_INIT(void)
{
    uint8_t buf;
    
    //1.Configure PortA
    buf=TRISA;
    TRISA=buf|0x10;//RA4
    ANSELA=0x10;
    //1.Configure PortB
    buf=TRISC;
    TRISC=buf|0x18;//RC3:ADC RC4
    ANSELC=0x18;
    
    
    //2.Configure the ADC module
    ADCON1bits.ADFM=1;      //Right justified.
    ADCON1bits.ADCS=0b101;  //FOSC/16  Select ADC conversion clock
    ADCON1bits.ADNREF=0;    //Vref-:AVss Configure voltage referrence 
    ADCON1bits.ADPREF=0b00; //Vref+ is connected to Vdd.
    ADCON0bits.CHS=0x13;    //RC3:Select ADC input channel
    
    //Turn on ADC module
    ADCON0bits.ADON=1;
    //3.Configure ADC interrupt
    //ADACTbits.ADACT=0b0000;//No auto-conversion trigger selected.
    ADACTbits.ADACT=0b00100; //Timer1 overflow 
    PIR1bits.ADIF=0;
    PIE1bits.ADIE=1;
    
    //ADC struct
    ADC.Ch=0x13;
    ADC.data_index=0;
    ADC.cal=0;
    ADC.readCompleted=false;
    //ADC2 struct
    ADC2.Ch=0x14;
    ADC2.readCompleted=false;
    //ADC3 struct
    ADC3.Ch=0x04;
    ADC3.readCompleted=false;
}
uint32_t ADC_RMS(void)
{
    uint8_t i,max_i, min_i;
    uint16_t max,min;
    uint32_t val;
    size_t nel, width;
    val=0;
    max=0x0000;
    min=0xFFFF;
    //qsort(ADC.data, sizeof ADC.data/sizeof ADC.data[0],sizeof ADC.data[0], sortem);
    for(i=0; i<ADC_Sampling_Num; i++)
    {
        val+=ADC.data[i];
    }
    val/=ADC_Sampling_Num;
    return val;
}
void ADC_Calibration(void)
{
    uint8_t i;
    uint16_t buf;
    
    T1CONbits.TMR1ON=0;  
    PIE1bits.ADIE=0;
    PIE1bits.TMR1IE=0;
    ADC.cal=0;
    i=0;
    ADCON0bits.CHS=0x13;
    do{
        __delay_us(100);
        ADCON0bits.GO_nDONE=1;
        while(ADCON0bits.GO_nDONE);
        buf=ADRESH;
        buf<<=8;
        buf|=ADRESL;
        ADC.cal+=buf;
        PIR1bits.ADIF=0;
        i++;
    }while(i<ADC_Sampling_Num);
    ADC.cal/=ADC_Sampling_Num;
  
   
    
    T1CONbits.TMR1ON=1;  
    PIE1bits.ADIE=1;
    PIE1bits.TMR1IE=1;
}

 //interrupt start
void Interrupt_START(void)
{
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
}

uint8_t  eepromWrite(uint8_t add, uint8_t byte)
{
    uint8_t cnt;
    cnt=0;
    NVMDATL = byte;
    NVMCON1bits.NVMREGS=1;
    NVMCON1bits.WREN =1;
    NVMADRH = 0x70;
    NVMADRL = add;
    
    INTCONbits.GIE=0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR=1;
    INTCONbits.GIE=1;
    while(WR && cnt<10)
    {
        __delay_us(10);
        cnt++;
     };  
     if(cnt>10)
         return 0;//time out error;
     else
         return 1;//write scucess
}

uint8_t eepromRead(uint8_t add)
{
    uint8_t buf;
    NVMCON1bits.NVMREGS =1;//eeprom accsess:1
    NVMADRL=add;
    NVMADRH=0x70;
    NVMCON1bits.RD = 1;
    buf=NVMDATL;
    return buf;
}