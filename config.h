/* 
 * File:   config.h
 * Author: Graham Smith
 *
 * Created on April 8, 2016, 11:45 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <p18f25K80.h>

// Rev 1.0 defines
//#define DIP1_TRIS TRISBbits.TRISB1
////#define DIP2_TRIS TRISBbits.TRISB2
////#define DIP3_TRIS TRISCbits.TRISC4
//
//#define DIP1_PORT PORTBbits.RB1
////#define DIP2_PORT PORTBbits.RB2
//#define DIP3_PORT PORTCbits.RC4
//
//#define GREEN1_TRIS TRISBbits.TRISB5
//#define RED1_TRIS TRISBbits.TRISB4
////#define BLUE1_TRIS TRISBbits.TRISB3
//
//#define GREEN1_LAT LATBbits.LATB5
//#define RED1_LAT LATBbits.LATB4
////#define BLUE1_LAT LATBbits.LATB3
//
//#define IR_DRIVER_TRIS TRISCbits.TRISC2
//#define IR_DRIVER_LAT LATCbits.LATC2
//
//#define IR1A_INPUT_TRIS TRISBbits.TRISB0
//#define IR1A_INPUT_PORT PORTBbits.RB0
//
//#define IR1B_INPUT_TRIS TRISAbits.TRISA1
//#define IR1B_INPUT_PORT PORTAbits.RA1
//
//#define STATUS1_TRIS TRISAbits.TRISA2
//#define STATUS1_LAT  LATAbits.LATA2
//
//#define STATUS2_TRIS TRISAbits.TRISA5
//#define STATUS2_LAT  LATAbits.LATA5

// DIP switch
#define DIP1_TRIS TRISCbits.TRISC1
#define DIP2_TRIS TRISCbits.TRISC0
#define DIP3_TRIS TRISCbits.TRISC7
#define DIP4_TRIS TRISBbits.TRISB4

#define DIP1_PORT PORTCbits.RC1
#define DIP2_PORT PORTCbits.RC0
#define DIP3_PORT PORTCbits.RC7
#define DIP4_PORT PORTBbits.RB4

// Light 1
#define WHITE_1_TRIS    TRISCbits.TRISC6
#define WHITE_1_LAT     LATCbits.LATC6

// Light 2
#define WHITE_2_TRIS    TRISBbits.TRISB5
#define WHITE_2_LAT     LATBbits.LATB5

// Status
#define STATUS_I_TRIS   TRISAbits.TRISA5
#define STATUS_I_LAT    LATAbits.LATA5
#define STATUS_J_TRIS   TRISAbits.TRISA3
#define STATUS_J_LAT    LATAbits.LATA3
#define STATUS_K_TRIS   TRISAbits.TRISA2
#define STATUS_K_LAT    LATAbits.LATA2

#define IR_DRIVER_TRIS TRISCbits.TRISC2
#define IR_DRIVER_LAT LATCbits.LATC2

#define IR1_INPUT_TRIS TRISBbits.TRISB1
#define IR1_INPUT_PORT PORTBbits.RB1

#define IR2_INPUT_TRIS TRISBbits.TRISB0
#define IR2_INPUT_PORT PORTBbits.RB0

#define SDO_TRIS    TRISCbits.TRISC5
#define SDO_LAT     LATCbits.LATC5

#define MAGIC_TRIS  TRISAbits.TRISA1
#define MAGIC_PORT   PORTAbits.RA1

void init_TRIS(void);

#endif	/* CONFIG_H */

