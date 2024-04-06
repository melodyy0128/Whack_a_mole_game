#include <p33FJ256MC710.h>

#include "joystick.h"

//initialization with ADC1
void joystick_init(void){
    CLEARBIT(AD1CON1bits.ADON);
    
    SETBIT(TRISEbits.TRISE8);
    SETBIT(AD1PCFGHbits.PCFG20);
    
    
    SETBIT(TRISBbits.TRISB4);
    SETBIT(TRISBbits.TRISB5);
    CLEARBIT(AD1PCFGLbits.PCFG4);
    CLEARBIT(AD1PCFGLbits.PCFG5);
    SETBIT(AD1CON1bits.AD12B);
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0x7;
    
    SETBIT(AD1CON1bits.ADON);
}
