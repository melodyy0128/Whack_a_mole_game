/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: template file to implement basic  */
/*                macros and functions to interface */
/*                with the analog joystick.         */
/*                                                  */
/****************************************************/

#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#include <p33Fxxxx.h>
#include "types.h"

/* Please NOTE: */
/* BTN1 (BUTTON 1) is the "fire" button on the koystick */
/* BTN2 (BUTTON 2) is the "thumb" button on the koystick */
#define BTN1_PRESSED() \
    (PORTEbits.RE8 == 0)

#define BTN1_RELEASED() \
    (PORTEbits.RE8 == 1)

#define BTN2_PRESSED() \
    (PORTDbits.RD10 == 0)

#define BTN2_RELEASED() \
    (PORTDbits.RD10 == 1)

/* === Function prototypes implemented in joystic.h go here === */
//initialization of motor channel
void joystick_init(void);


#endif /* __JOYSTICK_H */


