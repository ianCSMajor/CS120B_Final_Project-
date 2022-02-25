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

	enum LED_State {START, INIT, INC, INC_PUSH, DEC, DEC_PUSH, BOTH_PRESS} LED_STATE;

	void Tick(){
	switch(LED_STATE){
		case START:
			LED_STATE = INIT;
			break;
		case INIT:
			if((~PINA & 0x01) == 0x01){
			LED_STATE = INC_PUSH;
			}
			if((~PINA & 0x02) == 0x02){
			LED_STATE = DEC_PUSH;
			} 
			if((~PINA & 0x03) == 0x03){
			LED_STATE = BOTH_PRESS;
			}
			break;
		case INC_PUSH:
			LED_STATE = INC;
			break;
		case INC:
		if((~PINA & 0x03) == 0x01){
			LED_STATE = INC;
			}
			else{
			LED_STATE = INIT;
			}
			break;
		case DEC_PUSH:
			LED_STATE = DEC;
			break;
		case DEC:
			if((~PINA & 0x03) == 0x02){
			LED_STATE = DEC;
			}
			else{
			LED_STATE = INIT;
			}
			break;
		case BOTH_PRESS:
			if((~PINA & 0x03) == 0x03){
				LED_STATE = BOTH_PRESS;
				}
			else{
				LED_STATE = INIT;
			} 
			break;
		default:
			LED_STATE = INIT;
			break;
	}

	switch(LED_STATE){
		case START:
			PORTC = 0x07;
			break;
		case INIT:
			break;
		case INC:
			break;
		case INC_PUSH:
		if(PORTC < 0x09){
			PORTC = PORTC + 1;
			}
			break;
		case DEC: 
			break;
		case DEC_PUSH:
			if(PORTC > 0x00){
			PORTC = PORTC - 1;
			}
			break;
		case BOTH_PRESS:
			PORTC = 0x00;
			break;	
		default:
			PORTC = 0x07;
			break;
		}
}
int main(void){
DDRA = 0x00; 
PORTA = 0xFF;
DDRC = 0xFF;
PORTC = 0x07;
LED_STATE = START;
while(1){
	Tick();
}


return 1;
}
