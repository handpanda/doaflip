/********************************************************************
can.h

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

#ifndef __CAN_H__
#define __CAN_H__

#include "nutz.h"

#define CAN_TRANSMIT_BUFFER_LENGTH	8
#define CAN_RECEIVE_BUFFER_LENGTH	8

/*******************************************************************
 * Typedefs
 *******************************************************************/

typedef struct {
    union {
        uint16 sid;
    } id;
    bool rtr;
    uint8 length;
    uint8 data[8];
} canPacket;

/*******************************************************************
 * Public Functions
 *******************************************************************/

void can_init(void);
void can_send(canPacket* p);
void can_copyPacket(canPacket* s, canPacket* d);
void can_trySend(void);

void can_registerReceiveMethod(voidfxn fxn);
bool can_doReceive(canPacket *p);

#endif







