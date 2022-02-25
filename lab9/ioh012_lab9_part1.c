/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab # 9 Exercise # 1
 *  Demo Link: https://www.youtube.com/watch?v=7JDJSkpamNc
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum states {INIT, OFF, C_PUSH, D_PUSH, E_PUSH} state;

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
    if((~PINA & 0x01) == 0x01){
            state = C_PUSH;
        }
        else if((~PINA & 0x02) == 0x02){
            state = D_PUSH;
        }
        else if((~PINA & 0x04) == 0x04){
            state = E_PUSH;
        }
        else{
            state = INIT;
        }
    break;
    case C_PUSH:
        if((~PINA & 0x01) == 0x01){
            state = C_PUSH;
        } 
        else{
            state = OFF;
        }
        break;
     case D_PUSH:
        if((~PINA & 0x02) == 0x02){
            state = D_PUSH;
        } 
        else{
            state = OFF;
        }
        break;
     case E_PUSH:
        if((~PINA & 0x04) == 0x04){
            state = E_PUSH;
        } 
        else{
            state = OFF;
        }
        break;
    case OFF: 
        state = INIT;
        break;
        default:
        state = INIT;
        break;
}
switch(state){
    case INIT:
        break;
    case C_PUSH:
        if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
        if((~PINA & 0x01) == 0x01){
            set_PWM(261.63);
        } 
        break;
     case D_PUSH:
      if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
        if((~PINA & 0x02) == 0x02){
            set_PWM(293.66);
        } 
        break;
     case E_PUSH:
      if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
        if((~PINA & 0x04) == 0x04){
            set_PWM(329.63);
        } 
        break;
    case OFF:
        set_PWM(0);
        default:
        break;
}
}

int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    while(1){
        Tick();
    }
    return 1;
}