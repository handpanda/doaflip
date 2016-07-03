/********************************************************************
nutz.h - This file is the central header file for the Nutz OS.  Defines
         core functions and types to be use thoughout the code.  This file
         should be included in all other header files throughout the 
         application.
         
Copyright (c) 2011, Jonathan Nutzmann
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of UMNSVP, University of Minnesota nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY JONATHAN NUTZMANN ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL JONATHAN NUTZMANN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#ifndef __NUTZ_H__
#define __NUTZ_H__

#include "config.h"

/*****************************************************
 * Establish shorthand for commonly used types.
 *****************************************************/

typedef unsigned long 	uint32;
typedef unsigned int 	uint16;
typedef unsigned char 	uint8;

typedef signed char	int8;
typedef signed short    int16;
typedef signed long	int32;

typedef unsigned char 	bool;

typedef void(*voidfxn)(void);

#define true  1
#define false 0	



/*****************************************************
 * Define Interrupt Macros & ISR Handlers
 *****************************************************/

#define interrupts_en()			{ INTCONbits.GIE = 1;  }
#define interrupts_dis()		{ INTCONbits.GIE = 0;  }
#define interrupts_enPeripheral()	{ INTCONbits.PEIE = 1; }
#define interrupts_disPeripheral() 	{ INTCONbits.PEIE = 0; }
#define interrupts_enPriority() 	{ RCONbits.IPEN = 1;   }
#define interrupts_disPriority()	{ RCONbits.IPEN = 0;   }

#define MAX_ISR 5

#define NULL    0

/* To handle interrupt control, the os maintains a set of functions
 * that will be run every time an interrupt is generated.  To add to
 * these functions, simply use the functions register_high_isr or
 * register_low_isr and pass the function to be run as a parameter.
 */

void register_high_isr(voidfxn functionPointer);
void register_low_isr(voidfxn functionPointer);


/*****************************************************
 * Define location of 3 LEDs that are used to indicate state 
 * of the system.
 *****************************************************/
 
//#define NO_LIGHTS
 
 
#ifdef NO_LIGHTS

    #define led_g_on() LED_G_LAT = 0
    #define led_r_on() LED_R_LAT = 0
    #define led_y_on() LED_Y_LAT = 0
    #define led_b_on() LED_B_LAT = 0

    #define led_g_toggle() LED_G_LAT = 0
    #define led_r_toggle() LED_R_LAT = 0
    #define led_y_toggle() LED_Y_LAT = 0
    #define led_b_toggle() LED_B_LAT = 0

#endif
#ifndef NO_LIGHTS

    #define led_g_on() LED_G_LAT = 1
    #define led_r_on() LED_R_LAT = 1
    #define led_y_on() LED_Y_LAT = 1
    #define led_b_on() LED_B_LAT = 1

    #define led_g_toggle() LED_G_LAT ^= 1
    #define led_r_toggle() LED_R_LAT ^= 1
    #define led_y_toggle() LED_Y_LAT ^= 1
    #define led_b_toggle() LED_B_LAT ^= 1
	
#endif

#define led_g_off() LED_G_LAT = 0
#define led_r_off() LED_R_LAT = 0
#define led_y_off() LED_Y_LAT = 0
#define led_b_off() LED_B_LAT = 0

#define LED_R   0
#define LED_Y   1
#define LED_G   2

void led_init(void);
void led_updateFlashPattern(uint8 color, uint32 period);

#endif
