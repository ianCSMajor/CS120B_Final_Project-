/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Final Project - Part 1
 *	Exercise Description: [optional - include for your own benefit]
 *  Demo: https://www.youtube.com/watch?v=vFi25vZNx04
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "io.h"
#include <timer.h>
#include <keypad.h>
#include <taskScheduler.h>
#include <avr/interrupt.h>
// #include <bit.h>
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
    char buffer[20];
    int ADC_Value;
    ADC_init();
    LCD_init();
    while(1){

      
    ADC_Value = ADC;
    unsigned char my_char = (char)ADC_Value;
    LCD_DisplayString(1, ADC_Value);
    }
    return 1;
}