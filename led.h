/* 
 * File:   led.h
 * Author: graham
 *
 * Created on August 27, 2016, 10:37 AM
 */

#ifndef LED_H
#define	LED_H

void led_init();

void pwm_init();
void pwm_on(uint8 index);   
void pwm_off(uint8 index);
void pwm_update();
void pwm_fade(uint8 index, int8 step);

void writeLED(uint8 red, uint8 green, uint8 blue);
void delayPTime(uint8 multiplier);
void writeZero();
void writeOne();
void writeCommand();
void waitEOS();
void writeData(uint8 data);
void writeCommTimer();
void waitGSLAT();

#endif	/* LED_H */

