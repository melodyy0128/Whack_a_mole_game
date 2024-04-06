#include <p33Fxxxx.h>
#include "types.h"

//initialization of motor channel
void motor_init(uint8_t chan);

//set duty cycle, provided in microseconds
void motor_set_duty(uint8_t chan, uint16_t duty_us);
