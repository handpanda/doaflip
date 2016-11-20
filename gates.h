/* 
 * File:   gates.h
 * Author: graham
 *
 * Created on September 6, 2016, 10:26 PM
 */

#ifndef GATES_H
#define	GATES_H

#define GATES_PER_BOARD 2
#define MAX_BOARDS 16

#define FORWARD 0
#define BACKWARD 1

#define GATE_STATUS_OPEN 0
#define GATE_STATUS_CLOSED 1

void gate_init();
uint8 gate_getIdentifier();
void gate_tripGate(uint8 local_gate_index);
void gate_respondToGate(uint8 gate_index);
void gate_lightSection(uint8 section_index);

void gate_clearGate1();
void gate_clearGate2();
void gate_clearSection1();
void gate_clearSection2();

#endif	/* GATES_H */

