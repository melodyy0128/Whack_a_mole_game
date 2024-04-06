#include "timer.h"

//period in ms
void set_timer1(uint32_t period){
	T1CONbits.TON = 0; // Disable timer
    
    T1CONbits.TCS = 0; // Set 12.8mhz internal clock source
    T1CONbits.TGATE = 0; // Disable gated mode 
    T1CONbits.TSYNC = 1; // disable synchronization
    
    TMR1 = 0x00; // Reset timer value
    
    T1CONbits.TCKPS = 0b11; // Set 1:256 prescaler
    PR1 = period; // Set period (equivalent to ms until interrupt)
    
    IPC0bits.T1IP = 0x01; // Set interrupt priority
    IFS0bits.T1IF = 0; // Clear interrupt flag
    
    IEC0bits.T1IE = 1; // Enable interrupt
}

void set_timer2(uint32_t period){
	T2CONbits.TON = 0; // Disable timer
    
    T2CONbits.TCS = 0; // Set 12.8mhz internal clock source
    T2CONbits.TGATE = 0; // Disable gated mode 
//    T2CONbits.TSYNC = 0;
    
    TMR2 = 0x00; // Reset timer value
    
    T2CONbits.TCKPS = 0b11; // Set 1:256 prescaler
    PR2 = period; // Set period (equivalent to ms until interrupt)
    
    IPC1bits.T2IP = 0x01; // Set interrupt priority
    IFS0bits.T2IF = 0; // Clear interrupt flag
    
    IEC0bits.T2IE = 1; // Enable interrupt
}

void set_timer3(uint16_t period){
    // Configure timer 3 interrupts
    T3CONbits.TON = 0;
    
    // Configure timer3 for 0.02ms period and enable it
    T3CONbits.TCS = 0;
    T3CONbits.TGATE = 0;
    
    TMR3 = 0x00;
    
    T3CONbits.TCKPS = 0b11;
    PR3 = period;
    
    IPC2bits.T3IP = 0x02;
    IFS0bits.T3IF = 0;
    
    IEC0bits.T3IE = 1;   
}



