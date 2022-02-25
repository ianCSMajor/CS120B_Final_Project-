/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab # 8 Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *  Demo Link: https://www.youtube.com/shorts/fINGyWg20cU
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
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
    unsigned char max = 0xFF;
     ADC_init();
   
    //Value of ADC register is now stored in variable x 
    while(1){
    x = ADC;
    unsigned char my_char = (char)x;

    if(my_char >= max / 2){
        PORTB = 0x01;

    }
    else if (my_char < max / 2){
        PORTB = 0x00;
    }
    }
    return 1;
}