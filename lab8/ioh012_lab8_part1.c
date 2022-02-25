/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab # 8 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Demo Link: https://youtube.com/shorts/w9MsY4lLMtI?feature=share
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init(){
    ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    unsigned short x;
     ADC_init();
   
    //Value of ADC register is now stored in variable x 
    while(1){
    x = ADC;
    PORTB = (char)x;
    PORTD = (char) (x >> 8);

    }
}