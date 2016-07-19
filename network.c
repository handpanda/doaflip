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
    static int light = 0; 
    
    light ^= 1;
    
    set_status(light, light);
	sPacket.id.sid = NETWORK_ID_DATA_LIGHTSTATUS;
	sPacket.rtr = false;
	sPacket.length = 8;       
	
    sPacket.data[0] = 0;
	sPacket.data[1] = 1;
    sPacket.data[2] = 2;
    sPacket.data[3] = 3;
    sPacket.data[4] = 4;
    sPacket.data[5] = 5;
    sPacket.data[6] = 6;
    sPacket.data[7] = 7;
    
	can_send(&sPacket);    
}

void init_network() {    
    can_registerReceiveMethod(&network_receive);
}
