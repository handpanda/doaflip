#include "can.h"
#include "lights.h"
#include "network.h"
#include "can.h"
#include "gates.h"

void network_init() {    
    can_registerReceiveMethod(&network_receive);
}

void network_receive() {
    canPacket p;
	uint8 subcategory;
	
	can_doReceive(&p);
	
	switch (p.id.sid) {
        case NETWORK_ID_COMMAND_LIGHTSECTION:
            gate_lightSection(p.data[0]);
            
            break;
        case NETWORK_ID_DATA_GATETRIPPED:
            gate_respondToGate(p.data[0]);
            
            break;
        
		default:
            //toggle_lights();
		break;
	}	
}
int light = 0;

void network_sendLightStatus() {
 	canPacket sPacket;    
    
    light ^= 1;
    
    set_status(light, light, -1);
    
	return;
}

// Report a gate trip
void network_sendGatePacket(uint8 gate_index) {
    canPacket sPacket;
    
	sPacket.id.sid = NETWORK_ID_DATA_GATETRIPPED;
	sPacket.rtr = false;
	sPacket.length = 1;       
	
    sPacket.data[0] = gate_index;  
    
    can_send(&sPacket);
}

// Light up a section 
void network_lightSingleSection(uint8 section_index) {
    canPacket sPacket;
    
	sPacket.id.sid = NETWORK_ID_COMMAND_LIGHTSECTION;
	sPacket.rtr = false;
	sPacket.length = 1;       
	
    sPacket.data[0] = section_index;    
    
    can_send(&sPacket);
}