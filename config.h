/* 
 * File:   config.h
 * Author: Graham Smith
 *
 * Created on April 8, 2016, 11:45 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <p18f25K80.h>

#define DIP1_TRIS TRISBbits.TRISB1
#define DIP2_TRIS TRISBbits.TRISB2
#define DIP3_TRIS TRISCbits.TRISC4

#define DIP1_PORT PORTBbits.RB1
#define DIP2_PORT PORTBbits.RB2
#define DIP3_PORT PORTCbits.RC4

#define GREEN1_TRIS TRISBbits.TRISB5
#define RED1_TRIS TRISBbits.TRISB4
#define BLUE1_TRIS TRISBbits.TRISB3

#define GREEN1_LAT LATBbits.LATB5
#define RED1_LAT LATBbits.LATB4
#define BLUE1_LAT LATBbits.LATB3

#define IR_DRIVER_TRIS TRISCbits.TRISC2
#define IR_DRIVER_LAT LATCbits.LATC2

#define IR1A_INPUT_TRIS TRISBbits.TRISB0
#define IR1A_INPUT_PORT PORTBbits.RB0

#define IR1B_INPUT_TRIS TRISAbits.TRISA1
#define IR1B_INPUT_PORT PORTAbits.RA1

#define STATUS1_TRIS TRISAbits.TRISA2
#define STATUS1_LAT  LATAbits.LATA2

#define STATUS2_TRIS TRISAbits.TRISA5
#define STATUS2_LAT  LATAbits.LATA5

void init_TRIS(void);

#endif	/* CONFIG_H */

