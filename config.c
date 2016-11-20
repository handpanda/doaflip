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
    //RED1_TRIS = 0;
    //GREEN1_TRIS = 0;
    //BLUE1_TRIS = 0;
       
//    DIP1_TRIS = 1;
//    //DIP2_TRIS = 1;
//    //DIP3_TRIS = 1;
//    
//    IR_DRIVER_TRIS = 0;
//    
//    IR1A_INPUT_TRIS = 1;
//    IR1B_INPUT_TRIS = 1;
//    
//    STATUS1_TRIS = 0;
//    STATUS2_TRIS = 0;     
//    
//    SDO_TRIS = 0;
//    
//    WHITE_1_TRIS = 0;
    
    DIP1_TRIS = 1;
    DIP2_TRIS = 1;
    DIP3_TRIS = 1;
    DIP4_TRIS = 1;
    
    WHITE_1_TRIS = 0;
    WHITE_2_TRIS = 0;
    
    STATUS_I_TRIS = 0;
    STATUS_J_TRIS = 0;
    STATUS_K_TRIS = 0;

    IR_DRIVER_TRIS = 0;
    
    IR1_INPUT_TRIS = 1;
    IR2_INPUT_TRIS = 1;
    
    SDO_TRIS = 0;
    
    MAGIC_TRIS = 1; 
}

