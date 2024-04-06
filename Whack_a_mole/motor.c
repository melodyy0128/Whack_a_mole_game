/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab5   */
/*                                                  */
/****************************************************/


#include <p33FJ256MC710.h>

#include "motor.h"

//initialization of motor channel
void motor_init(uint8_t chan){
    CLEARBIT(T2CONbits.TON);
    
    CLEARBIT(T2CONbits.TCS);
    CLEARBIT(T2CONbits.TGATE);
    TMR2 = 0x00;
    T2CONbits.TCKPS = 0b10;
    CLEARBIT(IEC0bits.T2IE);
    CLEARBIT(IFS0bits.T2IF);
    PR2 = 4000;
    
    if(chan == 8) {
        CLEARBIT(TRISDbits.TRISD7);
        OC8R = 3700;
        OC8RS = 3580;
        OC8CONbits.OCM = 0b110;
    }
    else {
        CLEARBIT(TRISDbits.TRISD6);
        OC7R = 3700;
        OC7RS = 3580;
        OC7CONbits.OCM = 0b110;
    }
    
    SETBIT(T2CONbits.TON);
}

// 3820
// 3580

//set duty cycle, provided in microseconds
void motor_set_duty(uint8_t chan, uint16_t duty_us){
    uint32_t period = (20000 - duty_us) * 0.2;
    if(chan == 8) {
        OC8RS = period;
    }
    else {
        OC7RS = period;
    }
}

