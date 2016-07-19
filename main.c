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
#include "can.h"
#include "nutz.h"
#include "pwm.h"

// PIC18F25K80 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
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





/*
 * 
 */
void set_color(char r, char g, char b) {
    RED1_LAT = r;
    BLUE1_LAT = b;
    GREEN1_LAT = g;
}

// 0 open, 1 closed/blocked
bool gate_status = 0;


bool is_gate_open() {
    return gate_status;
}

bool gate_just_closed() {
    return !gate_status && IR1A_INPUT_PORT;
}

void update_gate_status() {
    if (IR1A_INPUT_PORT == 0) {
        gate_status = 0;
    }
}

void test_leds()
{
    static uint8 enable = 0;
    enable = (enable + 1) % 8;
    
    set_color(0, enable & 0b1, 0);
}

void toggle_blue() {
    static uint8 blue_status = 0;
    
    blue_status ^= 1;
    
    set_color(0, 0, blue_status);   
}

void ir_ISR (void) {
    if (INTCONbits.INT0IF) {
        INTCONbits.INT0IF = 0;   

       // if (gate_status == 0) {
            test_leds();
       // }
        
      //  gate_status = 1;
    }
}


int main(void) {
    
    rti_init();   
    queue_init();    
    can_init();
    
    init_network(); 
    
    // Oscillator configuration
    OSCCONbits.IRCF = 0b111; // Set internal oscillator (HF/MF/LF) and prescaler
    OSCCONbits.SCS = 0b10; // Use internal oscillator    
    
    // Disable A/D converter
    ADCON0bits.ADON = 0;
    ANCON0 = 0;
    ANCON1 = 0;
    
    // Disable Comparators
    CM2CON = 0;
    CM1CON = 0;    
    
    PMD0bits.CCP5MD = 1; // Disable CCP5
    
    T3CON = 0;
    T0CON = 0;                  
       
    //init_pwm();
    
    // Set up interrupt for INT0
    INTCONbits.INT0IF = 0;
    INTCONbits.INT0IE = 1;
    INTCON2bits.INTEDG0 = 0; // Falling edge (rising edge hits both edges?) 
    
    // Register the ISR
    //.register_high_isr(&ir_ISR);

    // Init the outputs
    init_TRIS();
        
    //set_color(0, 0, 1);
    
    //rti_register(&toggle_lights, 500, -1, true);
    
    // Enable interrupts
    RCONbits.IPEN = 1;
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    set_color(0, 1, 0); 
    set_status(0, 0);
    if (DIP1_PORT) {
        rti_register(&network_sendLightStatus, 1000, -1, true);       
    }
    
    
    while (1) {
        queue_pump();             
    }
    
    // Should never hit this.
    set_color(1,0,0);
    
    return 0;
}