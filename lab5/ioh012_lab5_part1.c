/*	Author: Ian Oh
 *	Lab Section: 22
 *	Assignment: Lab # 2 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "avr/sleep.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
       	DDRC = 0xFF; PORTC = 0x00;

	unsigned char button = 0x00;
	unsigned char led = 0x00;

	while(1){
		button = ~PINA;
		

		if(button == 0x00){
		led = 0x40;
		}
		if(button == 0x01 || button == 0x02){
		led = 0x20;
		}
		if(button == 0x03 || button == 0x04){
		led = 0x30;
		}
		if(button == 0x05 || button == 0x06){
		led = 0x38;
		}
		if(button >= 7 && button <= 9){
		led = 0x3C;
		}
		if(button >=10 && button <=12){
		led = 0x3E;
		}
		if(button >= 13 && button <= 15){
		led = 0x3F;
		}
		if(button <= 4){
		led = led | 0x40;
		}
		if(button > 15){
		led = 0x70;	
		}
		PORTC = led;
	}
	return 1;
}
