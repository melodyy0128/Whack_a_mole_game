#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include "types.h"
#include <libpic30.h>
#include <uart.h>
#include "touch.h"

void touch_init(void){
    CLEARBIT(TRISEbits.TRISE1);
    CLEARBIT(TRISEbits.TRISE2);
    CLEARBIT(TRISEbits.TRISE3);
    
    SETBIT(LATEbits.LATE1);
    SETBIT(LATEbits.LATE2);
    CLEARBIT(LATEbits.LATE3);
    
    CLEARBIT(AD2CON1bits.ADON);
    SETBIT(TRISBbits.TRISB9);
    SETBIT(TRISBbits.TRISB15);
    CLEARBIT(AD2PCFGLbits.PCFG9);
    CLEARBIT(AD2PCFGLbits.PCFG15);
    SETBIT(AD2CON1bits.AD12B);
    AD2CON1bits.FORM = 0;
    AD2CON1bits.SSRC = 0x7;
    AD2CON2 = 0;
    //not necessary to set AD2CON3 or CON4 
    SETBIT(AD2CON1bits.ADON);
}


// dimension 0 = x; dimension 1 = y
void touch_select_dim_nodelay(uint8_t dimension){
    if(dimension) { //read y-coord
        SETBIT(LATEbits.LATE1);
        CLEARBIT(LATEbits.LATE2);
        CLEARBIT(LATEbits.LATE3);
        
        AD2CHS0bits.CH0SA = 0x9; //read y-coord
    }
    else { //read x-coord
        CLEARBIT(LATEbits.LATE1);
        SETBIT(LATEbits.LATE2);
        SETBIT(LATEbits.LATE3);
        
        AD2CHS0bits.CH0SA = 0xF; //read x-coord
    }
}

void touch_select_dim(uint8_t dimension){
    touch_select_dim_nodelay(dimension);
    __delay_ms(15); // Delay until reading is stable
}


uint16_t touch_read(void){
    SETBIT(AD2CON1bits.SAMP); //start samp
    while(!AD2CON1bits.DONE); //loop until done
    CLEARBIT(AD2CON1bits.DONE); //clear done bit
    return ADC2BUF0;
}

