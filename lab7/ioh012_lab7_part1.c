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
    volatile unsigned char TimerFlag = 0;
    void TimerISR() { TimerFlag = 1;}
    unsigned long _avr_timer_M = 1;
    unsigned long _avr_timer_cntcurr = 0;
    void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
	void Tick(){
         unsigned char temp = 0x00;

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
            PORTB = 0x00;
			break;
		case INIT:
            if(PORTB == 0x00){
               LCD_DisplayString(1, "0");
            }
            if(PORTB == 0x01){
                LCD_DisplayString(1, "1");
            }
              if(PORTB == 0x02){
                LCD_DisplayString(1, "2");
            }
              if(PORTB == 0x03){
                LCD_DisplayString(1, "3");
            }
              if(PORTB == 0x04){
                LCD_DisplayString(1, "4");
            }
              if(PORTB == 0x05){
                LCD_DisplayString(1, "5");
            }
              if(PORTB == 0x06){
                LCD_DisplayString(1, "6");
            }
             if(PORTB == 0x07){
                LCD_DisplayString(1, "7");
            }
             if(PORTB == 0x08){
                LCD_DisplayString(1, "8");
            }
             if(PORTB == 0x09){
                LCD_DisplayString(1, "9");
            }
			break;
		case INC:
			break;
		case INC_PUSH:
		if(PORTB < 0x09){
            // temp = temp + 1;
            PORTB = PORTB + 1;
        //  LCD_WriteData(temp + '0');
			}
			break;
		case DEC: 
			break;
		case DEC_PUSH:
			if(PORTB > 0x00){
                 PORTB = PORTB - 1;
			}
			break;
		case BOTH_PRESS:
			PORTB = 0x00;
			break;	
		default:
			PORTB = 0x00;
			break;
		}
}
int main(void){
DDRA = 0x00; PORTA = 0xFF;
// DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;
TimerSet(10);
TimerOn();
LCD_init();

//LED_STATE = START;
while(1){
	Tick();
    while(!TimerFlag){};
    TimerFlag = 0;

}


return 1;
}