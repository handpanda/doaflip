/* 
 * File:   config.c
 * Author: graham
 *
 * Created on May 9, 2016, 3:37 PM
 */

#include "config.h"

/*
 * 
 */
void init_TRIS(void) {
    RED1_TRIS = 0;
    GREEN1_TRIS = 0;
    BLUE1_TRIS = 0;
    
    //DIP1_TRIS = 1;
    
    IR_DRIVER_TRIS = 0;
    
    IR1A_INPUT_TRIS = 1;
    IR1B_INPUT_TRIS = 1;
    IR2A_INPUT_TRIS = 1;
    IR2B_INPUT_TRIS = 1;
    
    LATCH_RESET_TRIS = 0;   
}

