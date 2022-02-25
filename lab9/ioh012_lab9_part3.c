/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab # 9 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *  Demo Link: https://www.youtube.com/watch?v=P5rh4yjkhI8
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum states {INIT, ON_OFF, UP, UP_PRESS, DOWN, DOWN_PRESS, OFF} state;
unsigned char flag = 0x00;
unsigned char count = 0x01;
unsigned char i = 0x00;

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


void set_PWM(double frequency){
    static double current_frequency;

    if(frequency != current_frequency){
        if(!frequency) { TCCR3B &= 0x08;}
        else{ TCCR3B |= 0x03; }

        if(frequency < 0.954) { OCR3A =0xFFFF;}

        else if(frequency > 31250) { OCR3A = 0x000;}

        else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

        TCNT3 = 0;
        current_frequency = frequency;
    }
}
void PWM_on(){
    TCCR3A = (1 << COM3A0);

    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

    set_PWM(0);
}
void PWM_off() {
    TCCR3A = 0x00;
    TCCR3B = 0x00;
} 
void Tick(){
switch(state){
    case INIT:
        if((~PINA & 0x02) == 0x02){
            state = UP;
        }
        else{
            state = INIT;
        }
    break;
     case UP:
        if( i < 50){
            state = UP;
        }
        else{
            state = INIT;
        }
        break;
        default:
        state = INIT;
        break;
}
switch(state){
    case INIT:
        set_PWM(0);
        i = 0;
        break;
        case UP:
            if(i == 2){
                set_PWM(329.63);
            }
            else if(i == 10){
                 set_PWM(293.66);
            }
            else if(i == 20){
                set_PWM(440.00);

            }
            else if( i == 30){
                set_PWM(392.00);
            }
            else if( i == 34){
                set_PWM(523.25);

            }
            else if( i == 50){
                set_PWM(493.88);
            }
                i++;
            
        break;
        default:
        break;

}
}
int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    PWM_on();
    while(1){
        Tick();
        while(!TimerFlag) {};
	TimerFlag = 0;
    }
    return 1;
}