#include "config.h"
#include "nutz.h"
#include "rti.h"

#include "gates.h"

uint8 num_gates = GATES_PER_BOARD * MAX_BOARDS;
uint8 gate_status[GATES_PER_BOARD * MAX_BOARDS];

uint8 gate_1_status = GATE_STATUS_OPEN;
uint8 gate_2_status = GATE_STATUS_OPEN;

uint8 identifier = 0;
uint8 section_1_index;
uint8 section_2_index;

void gate_init() {
    identifier = 0;
    
    if (DIP1_PORT) identifier += 1;
    if (DIP2_PORT) identifier += 2;
    if (DIP3_PORT) identifier += 4;
    if (DIP4_PORT) identifier += 8;

    section_1_index = identifier * GATES_PER_BOARD;
    section_2_index = identifier * GATES_PER_BOARD + 1;
}

uint8 gate_getIdentifier() {
    return identifier;
}

void gate_tripGate(uint8 local_gate_index) {
    // rti is software debounce to filter multiple triggers
    
    if (local_gate_index == 1) {
        if (gate_1_status == GATE_STATUS_OPEN) {
            gate_1_status = GATE_STATUS_CLOSED;
                       
            rti_register(&gate_clearGate1, 100, 1, false);
            
            if (identifier == 0) {
                gate_respondToGate(section_1_index);
            } else {
                network_sendGatePacket(section_1_index);
            }
        }
    } else if (local_gate_index == 2) {        
        if (gate_2_status == GATE_STATUS_OPEN) {
            gate_2_status = GATE_STATUS_CLOSED;
            
            rti_register(&gate_clearGate2, 100, 1, false);
            
            if (identifier == 0) {
                gate_respondToGate(section_2_index);
            } else {
                network_sendGatePacket(section_2_index);
            }
        }
    }
}

void gate_respondToGate(uint8 gate_index) {         
    static bool ball_direction = FORWARD;

    // -1: initial state (all gates are "in front" of the ball)
    static int8 last_gate = -1;    
    
    // -1: light no section
    int8 section_to_light = -1;   
/*    
    // Only first board calculates
    if (identifier > 0) {
        return;
    }
    
    if (gate_index >= num_gates) {
        return;
    }
    
// Determine ball direction
    
    // Ball moving forward
    if (last_gate < gate_index) {
        ball_direction = FORWARD;
        
    // Ball crosses one gate twice in a row (reverses direction)
    } else if (last_gate == gate_index) {
        if (ball_direction == FORWARD) {
            ball_direction = BACKWARD;
        } else {
            ball_direction = FORWARD;
        }
        
    // Ball moving backwards
    } else {
        ball_direction = BACKWARD;
    }       
    
// Determine which section to light
    
    if (ball_direction == FORWARD) {
        section_to_light = gate_index;    
    } else {        
        section_to_light = gate_index - 1;                
    }
  */
    section_to_light = gate_index;
    
    if (section_to_light >= 0) {
        // Special handling for section 0 
        if (section_to_light == section_1_index) {
            gate_lightSection(section_1_index);
        } else if (section_to_light == section_2_index) {
            gate_lightSection(section_2_index);
        
        // All other sections get transmitted
        } else {
            network_lightSingleSection(section_to_light);
        }
    }
}

void gate_lightSection(uint8 section_index) {
    if (section_index == section_1_index) {
        set_status(-1, 1, -1);        
        pwm_fade(0, 10);
        
        rti_register(&gate_clearSection1, 1000, 1, false);
        
    } else if (section_index == section_2_index) {
        set_status(-1, -1, 1);        
        pwm_fade(1, 10);
        
        rti_register(&gate_clearSection2, 1000, 1, false); 
    }
}

void gate_clearGate1() {
    gate_1_status = GATE_STATUS_OPEN;
}

void gate_clearGate2() {
    gate_2_status = GATE_STATUS_OPEN;
}

void gate_clearSection1() {
    set_status(-1, 0, -1);    
    pwm_fade(0, -1);    
    
}

void gate_clearSection2() {
    set_status(-1, -1, 0);
    pwm_fade(1, -1);
}