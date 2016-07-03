/********************************************************************
queue.c 

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
 
#include "queue.h"
#include "ringbuffer.h"

static voidfxn osQueueBuffer[QUEUE_SIZE];
static ringbuff queue;

/**
 * Initalizes the queue.
 */
void queue_init(void)
{
    ringbuff_init(&queue, (char*)osQueueBuffer, (ringbuff_size_t) (QUEUE_SIZE*sizeof(voidfxn)) );
}

/**
 * Pushes a function onto the queue.
 * @param fxn - Function to push onto the queue.
 */
void queue_push(voidfxn fxn)
{
    ringbuff_push_back_s(&queue, &fxn, sizeof(voidfxn));
}

/**
 * Runs the next function on the queue.
 * @return - true if funtion ran, false if queue was empty.
 */
bool queue_pump(void)
{	
    voidfxn f;

    if (ringbuff_pop_front_s(&queue, &f, sizeof(voidfxn)))
    {
        f();
        return true;
    }
    else
    {
        return false;
    }
}
