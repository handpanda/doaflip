#include <p18f25K80.h>

#include "pwm.h"

void init_pwm() {
    PR2 = 0b00011001; // Set duty cycle
    T2CON = 0b00000100;
    CCPR2L = 0b00001100;
    CCP2CON = 0b00111100;     
}
