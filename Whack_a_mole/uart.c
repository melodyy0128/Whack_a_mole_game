/****************************************************/
/*                                                  */
/*   CS-454/654 Embedded Systems Development        */
/*   Instructor: Renato Mancuso <rmancuso@bu.edu>   */
/*   Boston University                              */
/*                                                  */
/*   Description: lab3 UART2 init,TX,RX functions   */
/*                                                  */
/****************************************************/

#include "uart.h"
#include "types.h"

inline void uart2_init(uint16_t baud)
{
	/* Implement me please. */
    
    /*Disable UART for configuration*/
    CLEARBIT(U2MODEbits.UARTEN);
    
    /*Disable interrupts*/
    IEC1bits.U2RXIE = 0;
    IEC1bits.U2TXIE = 0;
    
    /*Clear interrupt flag bits*/
    IFS1bits.U2RXIF = 0;
    IFS1bits.U2TXIF = 0;
    
    /*Set IO pins*/
    TRISFbits.TRISF3 = 1;   //set as input UART2 RX pin
    TRISFbits.TRISF2 = 0;   //set as output UART2 TX pin
    
    /*baud rate*/
    U2MODEbits.BRGH = 0;    //set low speed baud rate
    U2BRG = (uint32_t)800000/baud - 1;           //set baud rate to input baud rate
    
    /*Operating settings and start port*/
    U2MODE = 0; //8-bit, no parity, 1 stop bit
    U2MODEbits.RTSMD = 0; //select simplex mode
    U2MODEbits.UEN = 0; //select simplex mode
    U2MODE |= 0x00;
    
    U2MODEbits.UARTEN = 1;  //enable UART
    U2STA = 0;
    U2STAbits.UTXEN = 1;    //enable UART TX
    
}

void uart2_send_8(uint8_t data)
{
	while(U2STAbits.UTXBF);
    U2TXREG = data;
    while(!U2STAbits.TRMT);
}

int8_t uart2_recv(uint8_t* data)
{
	if(U2STAbits.OERR) {
        U2STAbits.OERR = 0;
    }
    
    if(U2STAbits.URXDA) {
        *data = U2RXREG & 0x00FF;
        return 1;
    }
    
    return 0;
}
