/* 
 * File:   network.h
 * Author: graham
 *
 * Created on July 6, 2016, 9:42 PM
 */

#ifndef NETWORK_H
#define	NETWORK_H

#define NETWORK_ID_DATA_LIGHTSTATUS 0b01000111111

void network_receive();
void network_sendLightStatus(int status);

void init_network();

#endif	/* NETWORK_H */

