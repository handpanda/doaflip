#include "config.h"
#include "nutz.h"
#include "led.h"

void led_init() {
    SDO_LAT = 0;
}

void delayPTime(uint8 multiplier) {
    uint8 i;
    for (i = 0; i < multiplier; i++) {
        Nop();       
    }
}
/*
void writeLED(uint8 ledNum, uint8 red, uint8 green, uint8 blue) {
    //for (int i = 0; i< numLeds; i++) {
        writeCommTimer();
        
    //   if (i = ledNum){
            writeCommand();
            writeData(red);
            writeData(green);
            writeData(blue);
     //   }
        waitEOS();
    //}
    waitGSLAT();
            
}
*/
void writeZero() {
    SDO_LAT = 1;
    Nop()
    SDO_LAT = 0;
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();   
}

void writeOne() {
    SDO_LAT = 1;
    Nop();
    SDO_LAT = 0;
    Nop();
    SDO_LAT = 1; //rising edge of second pulse has to be within 0.5 * tCycle
    Nop();
    SDO_LAT = 0;
    Nop();
}

void writeCommTimer() {
    //first two zeroes determine the timing (tcycle) after device is powered up or after a GSLAT
    writeZero();
    writeZero();
}

void writeCommand() {

    //write command in hex is 3AA, binary is 11 1010 1010
    writeOne();
    writeOne();
    
    writeOne();
    writeZero();
    writeOne();
    writeZero();

    writeOne();
    writeZero();
    writeOne();
    writeZero();    
}

//end of sequence (for a single driver chip)
void waitEOS() {
    SDO_LAT = 0;
    delayPTime(4*8); // min 3.5 to max 5.5 times tCycle
}

//grayscale latch (for the end of a chain of driver chips)
void waitGSLAT() {
    SDO_LAT = 0;
    delayPTime(10*8); // minimum 8 time tCycle
}
    
#define PULSE() SDO_LAT = 1; SDO_LAT = 0;
void _writeZero(){
    PULSE();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void _writeOne(){
    PULSE();
    Nop();
    PULSE();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
}

void writeWord(uint8 word){
    uint8 i;
    for(i = 0; i < 8; i++){
        if(word & 0x80)
            writeOne();
        else
            writeZero();
        word <<= 1;
    }
}

void writeLED(uint8 R, uint8 G, uint8 B){    
    uint16 i;
    uint8 word = 0x3A;
    uint8 words[4];
    
    words[0] = 0x3A;
    words[1] = R;
    words[2] = G;
    words[3] = B;    
    
    for(i = 0; i < 8; i++){
        if(word & 0x80)
            writeOne();
        else
            writeZero();
        word <<= 1;
    }       
    for(i = 0; i < 8; i++){
        if(R & 0x80)
            writeOne();
        else
            writeZero();
        R <<= 1;
    }        
    for(i = 0; i < 8; i++){
        if(G & 0x80)
            writeOne();
        else
            writeZero();
        G <<= 1;
    }       
    for(i = 0; i < 8; i++){
        if(B & 0x80)
            writeOne();
        else
            writeZero();
        B <<= 1;
    }    
    waitEOS();
    waitGSLAT();
}

void writeData(uint8 data) {
    uint8 i;
    
    for (i = 0; i < 8; i++) {
        if (data & 0b10000000) {
            writeOne();
        } else {
            writeZero();
        }
        data <<= 1;
    }
}

int8 dc_step[2];
int16 duty_cycle[2];

int16 dc_max = 104; // Brightness seems linear up to here
                    // Higher than this there is no discernible change
                    // Probably varies based on the LED

void pwm_on(uint8 index) {
    duty_cycle[index] = dc_max;
    dc_step[index] = 0;
}

void pwm_off(uint8 index) {
    duty_cycle[index] = 0;
    dc_step[index] = 0;
}

void pwm_update() {           
    int i;
    
    for (i = 0; i < 2; i++) {
        if (dc_step[i] != 0) {
            duty_cycle[i] += dc_step[i];
            if (duty_cycle[i] >= dc_max) {
                duty_cycle[i] = dc_max;                
                
                dc_step[i] = 0;
            }
            if (duty_cycle[i] <= 0) {
                duty_cycle[i] = 0;               
                
                dc_step[i] = 0;
            }
        }
    }
                    
    CCPR3L =            duty_cycle[0] & 0b1111111100;
    CCP3CONbits.DC3B =  duty_cycle[0] & 0b0000000011;
    
    CCPR5L =            duty_cycle[1] & 0b1111111100;
    CCP5CONbits.DC5B =  duty_cycle[1] & 0b0000000011;
}

void pwm_init() {
    CCP3CONbits.CCP3M = 0b1100; // 0b11xx PWM mode
    CCPR3L = 0b00000000;        // Duty cycle, high 8 bits
    CCP3CONbits.DC3B = 0b00;    // Duty cycle, low 2 bits
    
    CCP5CONbits.CCP5M = 0b1100; // 0b11xx PWM mode
    CCPR5L = 0b00000000;        // Duty cycle, high 8 bits
    CCP5CONbits.DC5B = 0b00;    // Duty cycle, low 2 bits
    
    dc_step[0] = 0;
    dc_step[1] = 0;
    
    duty_cycle[0] = 0;
    duty_cycle[1] = 0;
    
    pwm_update();
}

void pwm_fade(uint8 index, int8 step) {       
    dc_step[index] = step;    
    
    pwm_update();
}