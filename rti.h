/********************************************************************
rti.h - This is a set of functions that allow a function to be run at 
        equal intervals for a given or unlimited number of times.

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
 
#ifndef __RTI_H__
#define __RTI_H__

#include "nutz.h"

/*******************************************************************
 * Defines
 *******************************************************************/
 
#define RTI_IF	PIR4bits.TMR4IF
#define RTI_IE	PIE4bits.TMR4IE

#define MAX_RTI	10

/*******************************************************************
 * Typedefs
 *******************************************************************/

typedef struct {
    voidfxn function;
    uint32 interval;
    int repeat;
    uint32 tic;
    uint16 count;
    bool queue;
} rti;

/*******************************************************************
 * Public Functions
 *******************************************************************/

void rti_init(void);
void rti_register(voidfxn function, uint32 interval, int repeat, bool queue );
void rti_remove(voidfxn fxn);
bool rti_isRegistered(voidfxn function);
bool rti_updateInterval(voidfxn function, uint32 interval);
bool rti_setTic(voidfxn function, uint32 tic);
#endif