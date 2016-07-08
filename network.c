#include "can.h"
#include "lights.h"
#include "network.h"

void network_receive() {
    canPacket p;
	uint8 subcategory;
	
	can_doReceive(&p);
	
	switch (p.id.sid) {
        //case NETWORK_ID_DATA_LIGHTSTATUS
		default:
            toggle_lights();
		break;
	}	
}

void network_sendLightStatus() {
	canPacket sPacket;
    
    set_status(1, -1);
	sPacket.id.sid = NETWORK_ID_DATA_LIGHTSTATUS;
	sPacket.rtr = false;
	sPacket.length = 1;       
	
	sPacket.data[0] = 0;
	can_send(&sPacket);
    set_status(0, -1);
}

void init_network() {    
    can_registerReceiveMethod(&network_receive);
}
