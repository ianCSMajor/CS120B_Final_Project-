/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab # 8 Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 *  Demo Link: https://www.youtube.com/shorts/Y5AjYl4jB1M
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
        PORTB = 0xFF;
    }
    if(my_char >= max / 3){
        PORTB = 0x7F;
    }
      if(my_char >= max / 4){
        PORTB = 0x3F;
    }
      if(my_char >= max / 5){
        PORTB = 0x1F;
    }
      if(my_char >= max / 6){
        PORTB = 0x0F;
    }
      if(my_char >= max / 7){
        PORTB = 0x07;
    }
      if(my_char >= max / 8){
        PORTB = 0x03;
    }
      if(my_char >= max / 9){
        PORTB = 0x01;
    }
    else{
        PORTB = 0x00;
    }
    }
    return 1;
}