/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: Lab5                              */
/****************************************************/

#include <p33Fxxxx.h>
//do not change the order of the following 3 definitions
#define FCY 12800000UL 
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include <libpic30.h>
#include <uart.h>
#include "lcd.h"
#include "motor.h"
#include "touch.h"
#include "timer.h"
#include "joystick.h"

/* Initial configuration by EE */
// Primary (XT, HS, EC) Oscillator with PLL
_FOSCSEL(FNOSC_PRIPLL);

// OSC2 Pin Function: OSC2 is Clock Output - Primary Oscillator Mode: XT Crystal
_FOSC(OSCIOFNC_OFF & POSCMD_XT); 

// Watchdog Timer Enabled/disabled by user software
_FWDT(FWDTEN_OFF);

// Disable Code Protection
_FGS(GCP_OFF);  

#define SERVO_MIN_DUTY_US 900
#define SERVO_MAX_DUTY_US 2100
#define NUM_SAMPLE 21
#define A_VALUES (float[]){1.0000, -1.1430, 0.4128}
#define B_VALUES (float[]){0.0675, 0.1349, 0.0675}

uint8_t btn_value = 1;
uint16_t num_opposing_btn_readings = 0;

//current touchscreen ball position
uint16_t ball_touch_pos_x = 0;
uint16_t ball_touch_pos_y = 0;

// touch screen values
int current_dim = 0;
int16_t x_max;
int16_t y_max;
int16_t x_min;
int16_t y_min;

// mole tracking
uint8_t mole_x = 0;
uint8_t mole_y = 0;
int mole_count = 0;

int score = 0;

uint8_t x_touch=0;
uint8_t y_touch=0;

uint16_t update_ball_touch_pos_x;
uint16_t update_ball_touch_pos_y;
// flag indicates whether the current mole has been matched
int matched = 0;

//cumulative count to count when the play will stop
int cumulative_count=0;

uint8_t received_mole = 0;
uint8_t received_score = 0;


//helper function for computing median of a list
int compare(const void *v1, const void *v2) {
    int value_1 = * ( (int*) v1);
    int value_2 = * ( (int*) v2);
    
    if(value_1 > value_2) {
        return 1;
    }
    else if(value_1 < value_2) {
        return -1;
    }
    else{
        return 0;
    }
}

int16_t simple_sample(void){
 
    int8_t i;
    int16_t pos[NUM_SAMPLE];
    for(i = 0; i < NUM_SAMPLE; i++) {
        pos[i] = touch_read();
    }
    qsort(pos, NUM_SAMPLE, sizeof(int16_t), compare);
    return pos[NUM_SAMPLE/2];
}

void update_btn_value() {
    if(btn_value != PORTEbits.RE8) {
        num_opposing_btn_readings++;
        
        if(num_opposing_btn_readings == 0x5) {
            btn_value ^= 0x01;
            num_opposing_btn_readings = 0;
        }
    } else {
        num_opposing_btn_readings = 0;
    }
}

// for debouncing purpose
void __attribute__((__interrupt__)) _T2Interrupt(void) {
    update_btn_value();
    CLEARBIT(IFS0bits.T2IF);
}

void __attribute__((__interrupt__)) _T3Interrupt(void) {
 // sample ball position
    if (current_dim == 0) {
//        update_ball_touch_pos_x = simple_sample();
//        touch_select_dim_nodelay(1);
//        lcd_locate(0,0);
//        lcd_printf_d("ball x: %d", ball_touch_pos_x);
//        update_touch_x();
//        ball_touch_pos_x = simple_sample();
        
        int i;
        for(i = 0; i < 40; i++) {
            update_ball_touch_pos_x = simple_sample();
            if(abs(update_ball_touch_pos_x - ball_touch_pos_x) < 100) {
               break;
            }
        }
        ball_touch_pos_x = touch_to_lcd_y(update_ball_touch_pos_x, x_min, x_max);
        touch_select_dim_nodelay(1);
        
        
    } else if (current_dim == 1) {
//        ball_touch_pos_y = simple_sample();
////        lcd_locate(0,1);
////        lcd_printf_d("ball y: %d", ball_touch_pos_y);
//    //        update_touch_y();
//    //        ball_touch_pos_y = simple_sample();
//        ball_touch_pos_y = touch_to_lcd_x(ball_touch_pos_y, y_min, y_max);
//        touch_select_dim_nodelay(0);
        int i;
        for(i = 0; i < 30; i++) {
            update_ball_touch_pos_y = simple_sample();
            if(abs(update_ball_touch_pos_y - ball_touch_pos_y) < 70) {
               break;
            }
        }
        ball_touch_pos_y = touch_to_lcd_x(update_ball_touch_pos_y, y_min, y_max);
        touch_select_dim_nodelay(0);
    }
//    current_dim = (current_dim + 1) % 2;
    current_dim ^= 1;
    if(uart2_recv(&received_mole)){
        mole_x = received_mole;
        uart2_send_8(1);
        while(!uart2_recv(&received_mole));
        mole_y = received_mole;
        if(mole_x>0 && mole_y>0){
            cumulative_count+=1;
            matched=0;
        }
    }
//    __delay_ms(2);
//    if(uart2_recv(&received_mole)){
//        mole_y = received_mole;
//        if(mole_x>0 && mole_y>0){
//            cumulative_count+=1;
//            matched=0;
//        }
//    }
    
    
    lcd_clear();
    drawCircle(ball_touch_pos_y, ball_touch_pos_x, 5, 1);
//    lcd_locate(0,5);
//    lcd_printf("mole x is %d", mole_x);
//    lcd_locate(0,6);
//    lcd_printf("mole y is %d", mole_y);
    lcd_locate(0,7);
    lcd_printf("cumulative_count %d", cumulative_count);
//    lcd_clear_block(ball_touch_pos_y-5, ball_touch_pos_x-5, ball_touch_pos_y+5, ball_touch_pos_x+5);
    
    if(mole_x>0 && mole_y>0){
        drawBox(mole_x-2, mole_y-2, mole_x+2, mole_y+2, 1);
        if(((ball_touch_pos_y+5) > (mole_x-2)) &&
       ((ball_touch_pos_y-5) < (mole_x-2)) && 
       ((ball_touch_pos_y+5) < (mole_x+2))){
        x_touch=1;
    }
    else if(((ball_touch_pos_y+5) > (mole_x+2))&&
            ((ball_touch_pos_y-5) < (mole_x-2))){
        x_touch=1;
    }
    else if(((ball_touch_pos_y+5) > (mole_x+2))&&
            ((ball_touch_pos_y-5) > (mole_x-2))&&
            ((ball_touch_pos_y-5) < (mole_x-2))){
        x_touch=1;
    }
    
    if(((ball_touch_pos_x+5) > (mole_y-2)) &&
       ((ball_touch_pos_x-5) < (mole_y-2)) && 
       ((ball_touch_pos_x+5) < (mole_y+2))){
        y_touch=1;
    }
    else if(((ball_touch_pos_x+5) > (mole_y+2))&&
            ((ball_touch_pos_x-5) < (mole_y-2))){
        y_touch=1;
    }
    else if(((ball_touch_pos_x+5) > (mole_y+2))&&
            ((ball_touch_pos_x-5) > (mole_y-2))&&
            ((ball_touch_pos_x-5) < (mole_y-2))){
        y_touch=1;
    }
    
    if(x_touch==1 && y_touch==1 && !matched){
        score+=1;
        matched = 1;
        lcd_locate(0,0);
        lcd_printf("scored %d", score);
    }
    lcd_locate(0,0);
    lcd_printf("scored %d", score);
    x_touch=0;
    y_touch=0;
    }
    
//    lcd_locate(0, 3);
//    lcd_printf("%d", ball_touch_pos_x);
    
    
    CLEARBIT(IFS0bits.T3IF);
}

// sample joystick value on ADC1
int sample_joystick_x(void){
    AD1CHS0bits.CH0SA = 0x4;
    SETBIT(AD1CON1bits.SAMP);
    while(!AD1CON1bits.DONE);
    CLEARBIT(AD1CON1bits.DONE);
    return ADC1BUF0;
}

int sample_joystick_y(void){
    AD1CHS0bits.CH0SA = 0x5;
    SETBIT(AD1CON1bits.SAMP);
    while(!AD1CON1bits.DONE);
    CLEARBIT(AD1CON1bits.DONE);
    return ADC1BUF0;
}

int joy_to_servo(uint16_t joy_min, uint16_t joy_max, uint16_t joy_reading){
    if(joy_reading < joy_min) {
        joy_reading = joy_min;
    }
    if(joy_reading > joy_max) {
        joy_reading = joy_max;
    }
    return (((float)SERVO_MAX_DUTY_US - SERVO_MIN_DUTY_US)/(joy_max - joy_min)
            * (joy_reading - joy_min) + SERVO_MIN_DUTY_US);
}

int touch_to_lcd_x(uint16_t ball_touch_pos, uint16_t min, uint16_t max){
    return ((ball_touch_pos-min)*((double)(127-0)/(max-min)));
}

int touch_to_lcd_y(uint16_t ball_touch_pos, uint16_t min, uint16_t max){
    return (63-0) - ((ball_touch_pos-min)*((double)(63-0)/(max-min)));
}

int main(){
	/* LCD Initialization Sequence */
	__C30_UART=1;	
	lcd_initialize();
    lcd_clear();
    uart2_init(9600);
    backLight(100);

//    while(1){
//        while(!uart2_recv(&received_mole));
//        lcd_clear();
//        lcd_printf("received! %x", received_mole);
//    }
    
//    while(1){
//        uart2_send_8(3);
//        lcd_locate(0,1);
//        lcd_printf("sent!");
//        
//    }
    
    
    // joystick initialization
    joystick_init();
    
    // timer2 for debouncing
    set_timer2(1);
    
    // set timer 3 for tracking ball position
    set_timer3(30 *50);
    
    // enable timer 2
    T2CONbits.TON = 1;
    
    uint16_t max_joystick_x;
    uint16_t min_joystick_x;
    uint16_t max_joystick_y;
    uint16_t min_joystick_y;
    
    while(btn_value) {
        max_joystick_x = sample_joystick_x();
        lcd_locate(0,0);
        lcd_printf_d("max x: %d", max_joystick_x);
    }
    while(!btn_value);
    
    while(btn_value) {
        min_joystick_x = sample_joystick_x();
        lcd_locate(0,1);
        lcd_printf_d("min x: %d", min_joystick_x);
    }
    while(!btn_value);
        
    while(btn_value) {
        max_joystick_y = sample_joystick_y();
        lcd_locate(0,2);
        lcd_printf_d("max y: %d", max_joystick_y);
    }
    while(!btn_value);
    
    while(btn_value) {
        min_joystick_y = sample_joystick_y();
        lcd_locate(0,3);
        lcd_printf_d("min y: %d", min_joystick_y);
    }
    while(!btn_value);
   
    // disable timer2 (debouncing)
    T2CONbits.TON = 0;
    lcd_clear();
    
    uint16_t duty_x_us; 
    uint16_t duty_y_us;
    uint16_t desired_x;
    uint16_t desired_y;
    
    motor_init(7);
    motor_init(8);
    motor_set_duty(7, 1500);
    motor_set_duty(8, 1500);
    
    touch_init();
    
    
//    motor_set_duty(7, SERVO_MAX_DUTY_US);
//    motor_set_duty(8, SERVO_MAX_DUTY_US);
    
    __delay_ms(2000);
            
//    // find min and max of touch screen
//    touch_select_dim(0);
//    x_max = simple_sample();
//    __delay_ms(15);
//    touch_select_dim(1);
//    y_max = simple_sample();    
//    __delay_ms(15);
    x_max = 2800;
    y_max = 2500;
    
    lcd_locate(0,0);
    lcd_printf("%d", x_max);
    lcd_locate(0,1);
    lcd_printf("%d", y_max);

    
//    motor_set_duty(8, 1200);
//    motor_set_duty(7, 1200);
//    motor_set_duty(7, SERVO_MIN_DUTY_US);
//    motor_set_duty(8, SERVO_MIN_DUTY_US);
//    
//    __delay_ms(2000);
//    touch_select_dim(0);
//    __delay_ms(15);
//    x_min = simple_sample();
//    touch_select_dim(1);
//    __delay_ms(15);
//    y_min = simple_sample();
    
    __delay_ms(1000);
    x_min = 380;
    y_min = 280;
    
    lcd_locate(0,2);
    lcd_printf("%d", x_min);
    lcd_locate(0,3);
    lcd_printf("%d", y_min);
    
    lcd_locate(0,4)
    lcd_printf("initialization done")
    __delay_ms(1000);
    
    lcd_clear();
    
    // enable timer 3(track ball & circle positions)
    T3CONbits.TON = 1;

	while(1){
        // joystick controls ball
//        SRbits.IPL = 0x7;
        //cumulative_count start from 1, will be 20 rounds game
        if(cumulative_count==21){
            //game end, clear lcd and stop timer
            T1CONbits.TON = 0;
            T3CONbits.TON = 0;
            lcd_clear();
            lcd_locate(0,0)
            lcd_printf("Game End!");
            break;
            
        }
        desired_x = sample_joystick_x();
        duty_x_us = joy_to_servo(min_joystick_x, max_joystick_x, desired_x);
        motor_set_duty(8, duty_x_us);
        desired_y = sample_joystick_y();
        duty_y_us = joy_to_servo(min_joystick_y, max_joystick_y, desired_y);
        motor_set_duty(7, duty_y_us);
//        SRbits.IPL = 0x0;

        //lcd_locate(0,5);
        //lcd_printf("%d,%d",duty_x_us,duty_y_us);
	}
    uart2_send_8(score);
    lcd_locate(0,1);
    lcd_printf("Your score is: %d", score);
    while(!uart2_recv(&received_score));
    if(received_score > score){
        lcd_locate(0,2);
        lcd_printf("You lost :(");
    }
    else if(received_score == score){
        lcd_locate(0,2);
        lcd_printf("TIE");
    }
    else {
        lcd_locate(0,2);
        lcd_printf("You won :)");
    }
    
     while(1){
        
    }
}


