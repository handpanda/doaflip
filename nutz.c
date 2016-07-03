/********************************************************************
nutz.c - This file is the central file for the Nutz OS.  Defines
         core functions and types to be use thoughout the code. 
         
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

#include "nutz.h"
#include "rti.h"
#include "config.h"

/*
 * To handle interrupt control, the os maintains a set of functions
 * that will be run every time an interrupt is generated.  To add to
 * these functions, simply use the functions register_high_isr or
 * register_low_isr and pass the function to be run as a parameter.
 */

volatile uint8  registered_high_isr = 0;
volatile voidfxn high_isr_functions[MAX_ISR];

volatile uint8  registered_low_isr = 0;
volatile voidfxn low_isr_functions[MAX_ISR];

/***************************************************************
 * ISR entries - These are placed at 0x08 and 0x18.  The simply
 * call the high_isr and low_isr functions.
 ***************************************************************/

void high_isr(void);
void low_isr(void);

#pragma code high_isr_entry=0x08
void high_isr_entry(void)
{
    _asm goto high_isr _endasm
}

#pragma code low_isr_entry=0x18
void low_isr_entry(void)
{
    _asm goto low_isr _endasm
}


/***************************************************************
 * ISR functions - Run the list of ISRs registered for both
 * high and low interrupts.
 ***************************************************************/

#pragma interrupt high_isr
void high_isr(void)
{
    uint8 i;
    
    for (i = 0; i < registered_high_isr; i++)
    {
        i = 0;
        high_isr_functions[i]();
    }
}

#pragma interrupt low_isr
void low_isr(void)
{
    int i;

    for (i = 0; i < registered_low_isr; i++)
    {
        low_isr_functions[i]();
    }
}

/**
 * Registers a low priority ISR routine.
 * @param functionPointer - Function to call when interrupt occurs.
 */
void register_low_isr(voidfxn functionPointer)
{
    if (registered_low_isr < MAX_ISR)
    {
        low_isr_functions[registered_low_isr] = functionPointer;
        registered_low_isr++;
    }
    else
    {
        //error_throw(ERROR_ISR_LOW_COUNT_OVERFLOW);
    }
}

/**
 * Registers a high priority ISR routine.
 * @param functionPointer - Function to call when interrupt occurs.
 */
void register_high_isr(voidfxn functionPointer)
{
    if (registered_high_isr < MAX_ISR)
    {
        high_isr_functions[registered_high_isr] = functionPointer;
        registered_high_isr++;
    }
    else
    {
        //error_throw(ERROR_ISR_HIGH_COUNT_OVERFLOW);
    }
}
