/********************************************************************
can.c

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

#include "config.h"

#include "nutz.h"
#include "can.h"
#include "rti.h"
#include "ringbuffer.h"
#include "queue.h"

/*******************************************************************
 * Global Variables
 *******************************************************************/

#define CAN_BUFFER_LENGTH        (8)   // In CANPackets
#define CAN_PACKET_SIZE             (sizeof(canPacket))
#define CAN_BUFFER_LENGTH_BYTES  ((ringbuff_size_t)(CAN_BUFFER_LENGTH * CAN_PACKET_SIZE))

static ringbuff txBuffer;
static char txBuff[CAN_BUFFER_LENGTH_BYTES];

static ringbuff rxBuffer;
static char rxBuff[CAN_BUFFER_LENGTH_BYTES];

static uint8 can_id;

voidfxn receiveFxn;

/********************************************************************
 * Defines
 ********************************************************************/

#define CAN_TX_TRIS		TRISCbits.TRISC6
#define CAN_RX_TRIS		TRISCbits.TRISC7

#define CAN_MODE_CONFIG				0b100
#define CAN_MODE_LOOPBACK			0b010
#define CAN_MODE_LISTEN				0b011
#define CAN_MODE_DISABLED_SLEEP                 0b001
#define CAN_MODE_NORMAL				0b000

#define CAN_MODE_LEGACY				0b00
#define CAN_MODE_ENHANCED_LEGACY                0b01
#define CAN_MODE_ENHANCED_FIFO                  0b10

#define CAN_PRIORITY_3				0b11 // highest
#define CAN_PRIORITY_2				0b10
#define CAN_PRIORITY_1				0b01
#define CAN_PRIORITY_0				0b00 // lowest

#define CAN_TXIF0		TXB0CONbits.TXBIF
#define CAN_TXIF1		TXB1CONbits.TXBIF
#define CAN_TXIF2		TXB2CONbits.TXBIF

#define CAN_TXREQ0		TXB0CONbits.TXREQ
#define CAN_TXREQ1		TXB1CONbits.TXREQ
#define CAN_TXREQ2		TXB2CONbits.TXREQ

/********************************************************************
 * ISRs
 ********************************************************************/	

void can_ISR(void) 
{
    canPacket packet;

    if (RXB0CONbits.RXFUL)
    {
        // This recieve buffer handles the PRG/CMD category packets

        packet.id.sid = RXB0SIDHbits.SID;
        packet.id.sid = packet.id.sid << 3;
        packet.id.sid += RXB0SIDLbits.SID;
        packet.rtr = RXB0DLCbits.RXRTR;
        packet.length = RXB0DLCbits.DLC;
        packet.data[0] = RXB0D0;
        packet.data[1] = RXB0D1;
        packet.data[2] = RXB0D2;
        packet.data[3] = RXB0D3;
        packet.data[4] = RXB0D4;
        packet.data[5] = RXB0D5;
        packet.data[6] = RXB0D6;
        packet.data[7] = RXB0D7;

        ringbuff_push_back_s(&rxBuffer, &packet, CAN_PACKET_SIZE);

        queue_push(receiveFxn);

        RXB0CONbits.RXFUL = 0;
    }

    if( PIR5bits.ERRIF)
    {
        PIR5bits.ERRIF = 0;
        if(COMSTATbits.RXB1OVFL)
        {
            COMSTATbits.RXB1OVFL = 0;
        }
    }

    // RXFUL is set when buffer full - must be cleared
    // FILHIT says which filter was hit
    // RXM defines receive modes! = 0b11 to accept all packets
    // RXB0 has 2 acceptance filters (used for higher priority)

    // This interrupt isn't running for some reason.
    //if (PIR5bits.TXB0IF)
    //{
    //	PIR5bits.TXB0IF = 0;
    can_trySend();
    //}
}		

/*******************************************************************
 * Function Definitions
 *******************************************************************/

void can_init(void) 
{
    // Configure receive port
    CAN_RX_TRIS = 1;

    // Put the device in config mode
    CANCONbits.REQOP = CAN_MODE_CONFIG;
    while(CANSTATbits.OPMODE != CAN_MODE_CONFIG); // Wait for config mode

    // ====== Put the device in legacy mode (for now) ======
    ECANCONbits.MDSEL = CAN_MODE_ENHANCED_LEGACY;

    // =========== Baudrate & Sampling Configuration ===========
    // Baudrate = 125kbps
    // Fosc = 32 MHz

    BRGCON1bits.SJW = 0x00;		// Synchronization jump width = 1 TQ
    BRGCON1bits.BRP = 1; 		// brp = 2

    BRGCON2bits.SEG1PH = 0x05; 	// Phase Segment 1 time = 7TQ
    BRGCON2bits.PRSEG = 0x03;  	// Propagation time = 1TQ
    BRGCON2bits.SAM = 1;		// triple sample bus
    BRGCON2bits.SEG2PHTS = 1;	// enable programming segph2

    BRGCON3bits.WAKDIS = 1;		// Disable Bus Wakeup
    BRGCON3bits.SEG2PH = 0x04;  // Phase Segment 2 time = 3TQ

    // =========== Configure the IO characteristics ===========

    CIOCONbits.ENDRHI = 1;		// Drive VDD when recessive for diff bus.
    CIOCONbits.CLKSEL = 1;		// Use Oscillator as the source of CAN clock.
    CIOCONbits.CANCAP = 0;		// CAN capture NOT enabled.

    // =========== Tranciever Setup ===========

    PIE5bits.TXB0IE = 1;
    TXB0CONbits.TXPRI = CAN_PRIORITY_3;

    ringbuff_init(&txBuffer, txBuff, CAN_BUFFER_LENGTH_BYTES);

    // =========== Receiver Setup ===========

    PIR5bits.RXB1IF = 0;
    PIR5bits.RXB0IF = 0;
    PIE5bits.RXB1IE = 1;
    PIE5bits.RXB0IE = 1;

    // Normal mode of operation.  Use acceptance filters.
    RXB0CONbits.RXM1 = 0;
    RXB1CONbits.RXM1 = 0;

    // Need to receive 3 categories of packets:
    // - PRG/CMD  : 0b 01 xxx xxxxxx (configured)
    // - DEBUG    : 0b 11 xxx xxxxxx (not configured)
    // - RTR/DATA : 0b 10 xxx xxxxxx (send only)

    // See: http://umnsvp.dyndns.org/uberwiki/C3:Battery_Protection_and_Equalization

    // Read DIP switches to set CAN id 
    can_id = DIP1_PORT |
             DIP2_PORT << 1 |
             DIP3_PORT << 2 |
             DIP4_PORT << 3;      
            
    // FILTER 0,1 & MASK 0 - Accept PRG/CMD

    // Specific Module - ID: MODULE_NUMBER
    RXF0SIDHbits.SID = 0b01000000 + ((can_id >> 3) & 0b111);
    RXF0SIDLbits.SID = (can_id & 0b111);
    RXF0SIDLbits.EXIDEN = 0;

    // All module command - ID: 0b111111
    RXF1SIDHbits.SID = 0b01000111;
    RXF1SIDLbits.SID = 0b111;
    RXF1SIDLbits.EXIDEN = 0;

    // Pay attention to the category and module id.
    RXM0SIDHbits.SID = 0b11000111;
    RXM0SIDLbits.SID = 0b111;

    RXFBCON0 = 0x00;  		// Associtates receive filter 0 & 1 with RXB0.
    MSEL0bits.FIL0 = 0b00;	// Use Mask 0 with Filter 0
    MSEL0bits.FIL1 = 0b00;  //     "           Filter 1

    RXFCON0bits.RXF0EN = 1; // Enable Filter 0
    RXFCON0bits.RXF1EN = 1; // Enable Filter 1


    // Make sure there aren't any pending messages.
    RXB0CONbits.RXFUL = 0;
    RXB1CONbits.RXFUL = 0;

    ringbuff_init(&rxBuffer, rxBuff, CAN_BUFFER_LENGTH_BYTES);


    // Register the interrupt
    register_high_isr(&can_ISR);

    // Set to normal mode...

    CANCONbits.REQOP = CAN_MODE_NORMAL;
    
    while(CANSTATbits.OPMODE != CAN_MODE_NORMAL);

    CAN_TXREQ0 = 0;
}	

// =========================================================

void can_registerRecieveMethod(voidfxn fxn) 
{
    receiveFxn = fxn;
}	

// =========================================================
void can_send(canPacket* p) 
{
    ringbuff_push_back_s(&txBuffer, p, CAN_PACKET_SIZE);
    can_trySend();
}

// =========================================================

// Attempts to send a packet off the queue, if a transmit buffer is free.
void can_trySend(void)
{
    canPacket p;

    // Use the TX0 registers to send general packets.  TX1 and TX2 will be configured
    // for RTR in the future.

    if (!CAN_TXREQ0)
    {

        // Try to pull a packet from the queue.  If it is empty, return.
        if (!ringbuff_pop_front_s(&txBuffer, &p, CAN_PACKET_SIZE)) { return; }

        TXB0SIDLbits.SID = (p.id.sid) & 0b0111;
        TXB0SIDH = (p.id.sid) >> 3;
        TXB0SIDLbits.EXIDE = 0;
        TXB0D0 = (p.data)[0];
        TXB0D1 = (p.data)[1];
        TXB0D2 = (p.data)[2];
        TXB0D3 = (p.data)[3];
        TXB0D4 = (p.data)[4];
        TXB0D5 = (p.data)[5];
        TXB0D6 = (p.data)[6];
        TXB0D7 = (p.data)[7];
        TXB0DLCbits.TXRTR = 0;
        TXB0DLCbits.DLC = p.length;

        CAN_TXREQ0 = 1; 				// Send the message
    }
}		

bool can_doReceive(canPacket *p) 
{
    return ringbuff_pop_front_s(&rxBuffer, p, CAN_PACKET_SIZE);
}		
