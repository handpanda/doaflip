/********************************************************************
rti.c

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
#include "queue.h"

/*******************************************************************
 * Public Variables
 *******************************************************************/

rti rtis[MAX_RTI];
voidfxn removeList[MAX_RTI];

volatile uint8 numActive = 0;
volatile uint8 removeCount = 0;

/*******************************************************************
 * Function Definitions
 *******************************************************************/


void rti_isr(void) 
{	
    int8 i,j;
    ClrWdt();
    if (RTI_IF && RTI_IE)
    {
        RTI_IF = 0;

        for (i = 0; i < numActive; i++)
        {
            rtis[i].tic++;
            if (rtis[i].tic >= rtis[i].interval)
            {
                if (rtis[i].queue)
                {
                    queue_push(rtis[i].function);
                }
                else
                {
                    rtis[i].function();
                }
                rtis[i].tic = 0;
                rtis[i].count++;
            }
        }
	
        for (i = numActive-1; i >= 0; i--)
        {
            if (rtis[i].repeat != -1 && rtis[i].repeat == rtis[i].count)
            {
                // RTI reached EOL, remove from list.
                numActive--;
                rtis[i].function = rtis[numActive].function;
                rtis[i].interval = rtis[numActive].interval;
                rtis[i].repeat = rtis[numActive].repeat;
                rtis[i].tic = rtis[numActive].tic;
                rtis[i].count = rtis[numActive].count;
                rtis[i].queue = rtis[numActive].queue;
            }

            for (j = removeCount-1; j >= 0; j--)
            {
                if (rtis[i].function == removeList[j])
                {
                    numActive--;
                    rtis[i].function = rtis[numActive].function;
                    rtis[i].interval = rtis[numActive].interval;
                    rtis[i].repeat = rtis[numActive].repeat;
                    rtis[i].tic = rtis[numActive].tic;
                    rtis[i].count = rtis[numActive].count;
                    rtis[i].queue = rtis[numActive].queue;
                    removeCount--;
                }
            }
        }
    }
}	

void rti_remove(voidfxn f)
{
    if (rti_isRegistered(f))
        removeList[removeCount++] = f;
}

void rti_register(voidfxn function, uint32 interval, int repeat, bool queue)
{
    rtis[numActive].function  = function;
    rtis[numActive].interval = interval;
    rtis[numActive].repeat = repeat; // if repeat = -1, go forever.
    rtis[numActive].tic = 0;
    rtis[numActive].count = 0;
    rtis[numActive].queue = queue;
    numActive++;
}

bool rti_setTic(voidfxn function, uint32 tic)
{
    uint8 i = 0;

    for (i=0; i < numActive; i++)
    {
        if (rtis[i].function == function)
        {
            rtis[i].tic = (tic % rtis[i].interval);
        }
    }
    return false;
}


bool rti_isRegistered(voidfxn function)
{
    uint8 i = 0;

    for (i=0; i < numActive; i++)
    {
        if (rtis[i].function == function)
            return true;
    }
    return false;
}

bool rti_updateInterval(voidfxn function, uint32 interval)
{
    uint8 i = 0;

    for (i=0; i < numActive; i++)
    {
        if (rtis[i].function == function)
        {
            rtis[i].interval = interval;
            rtis[i].tic = 0;
            return true;
        }
    }
    return false;
}

void rti_init( void )
{

    // Fosc/4 = 32 MHz/4 = 8MHz
    // 01 = Prescale of 4 -> 2MHz
    // 0111 = Postscale of 8 -> 250KHz
    // PR2 = 249 -> 1 KHz -> 1mS

    #define RTI_IF	PIR1bits.TMR2IF
    #define RTI_IE	PIE1bits.TMR2IE

    TMR2 = 0;
    PR2 = 249;
    T2CON = 0b00001111;

    RTI_IF = 0;
    RTI_IE = 1;

    register_high_isr(&rti_isr);
		  			
}
