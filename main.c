/* 
 * File:   main.c
 * Author: Graham Smith
 *
 * Created on April 8, 2016, 11:15 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <p18f25K80.h>
#include <timers.h>

#include "config.h"
#include "lights.h"
#include "network.h"

#include "ringbuffer.h"
#include "queue.h"
#include "rti.h"
#include "nutz.h"
#include "led.h"
#include "gates.h"

// PIC18F25K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set (Enabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)

// CONFIG3H
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)

// CONFIG5H
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)

// CONFIG6L
#pragma config WRT0 = OFF       // Table Write Protect 00800-01FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 02000-03FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 04000-05FFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 06000-07FFF (Disabled)

// CONFIG6H
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protect 00800-01FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 02000-03FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 04000-05FFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 06000-07FFF (Disabled)

// CONFIG7H
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

void ir_ISR() {
    // Gate 1
    if (INTCON3bits.INT1IF) {
        INTCON3bits.INT1IF = 0;        
        
        gate_tripGate(1);
        //set_status(-1, 1, -1);
        
        //rti_register(&gate_clearSection1, 500, 1, false);
    }
    
    // Gate 2
    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0;   

        gate_tripGate(2);
        //set_status(-1, -1, 1);
        
        //rti_register(&gate_clearSection2, 500, 1, false);        
    }    
}

void heartbeat() {
    static int8 status = 0;
    
    if (status == 0) {
        status = 1;
    } else {
        status = 0;
    }
    
    set_status(status, -1, -1);
}

int main(void) {
    
    init_TRIS();
    rti_init();   
    queue_init();    
    can_init();
    
    network_init(); 
    gate_init();
    pwm_init();
    
    set_status(1, 1, 1);
    
    // Oscillator configuration
    OSCCONbits.IRCF = 0b111; // Set internal oscillator (HF/MF/LF) and prescaler
    OSCCONbits.SCS = 0b10; // Use internal oscillator    
    
    // Disable A/D converter
    ADCON0bits.ADON = 0;
    ANCON0 = 0;
    ANCON1 = 0;                   
       
    // Interrupt 0 (gate 0)
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    
    // Interrupt 1 (gate 1)
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    
    INTCON2bits.INTEDG0 = 0; // Falling edge (rising edge hits both edges?) 
    
    // Register the ISR
    register_high_isr(&ir_ISR);

    PR2 =       0b01101000 ;
    T2CON =     0b00000100 ;
    CCPR2L =    0b00110100 ;
    CCP2CON =   0b00011100 ;        
    
//    rti_register(&heartbeat, 500, -1, false);
    
   // rti_register(&pwm_update, 10, -1, false);           
    
    // Enable interrupts
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    // Turn off I J K
    set_status(0, 0, 0);    
    
    gate_clearSection1();
    gate_clearSection2(); 
    
    WHITE_1_LAT = 0;
    WHITE_2_LAT = 0;           
    
    while (1) {       
        if (IR1_INPUT_PORT || IR2_INPUT_PORT) {
            set_status(1, -1, -1);
        } else {
            set_status(0, -1, -1);
        }
        
        
        
        if (!MAGIC_PORT) {            
     //       gate_tripGate(1);
        }
        
        queue_pump();             
    }   
    
    return 0;
}