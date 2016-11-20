/* 
 * File:   network.h
 * Author: graham
 *
 * Created on July 6, 2016, 9:42 PM
 */

#ifndef NETWORK_H
#define	NETWORK_H

#define NETWORK_ID_DATA_GATETRIPPED     0b01010101010
#define NETWORK_ID_COMMAND_LIGHTSECTION 0b01110000000

void network_init();

void network_receive();
void network_sendLightStatus(int status);

void network_sendGatePacket(uint8 gate_index);
void network_lightSingleSection(uint8 section_index);

#endif	/* NETWORK_H */

