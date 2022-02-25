  Your Name & E-mail: Ian Oh ioh012@ucr.edu
 *  Lab Section: 22
 *  Assignment: Lab # 7 Exercise # 2
 *  Exercise Description: [optional - include for your own benefit]
 *  
 *  I acknowledge all content contained herein, excluding template 
 *  or example code, is my own original work.
 *
 *  Demo Link: <URL>
 */

#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
    enum states {Start, Button, Button_Release, Init, NextLed} state;
    volatile unsigned char TimerFlag = 0;
    unsigned char score = 0x05;

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

    switch(state){
        case Start:
            state = Init;
            break;
        case Init:
        state = NextLed;
       break;
       case NextLed:
        if((~PINA & 0x01) == 0x01){
             state = Button;
        }
        else{
            state = NextLed;
        }
            break;
        case Button:
        if((~PINA & 0x01) == 0x01){
            state = NextLed;
        }
        else{
            state = Button;
        }
            break;
        default:
            state = Init;
            break;
    }
       switch(state){
        case Start:

            PORTB = 0x01;
            break;
        case Init:
                LCD_DisplayString(1, "5");

        

            break;
        case NextLed:
           
            if(PORTB == 0x04){
                PORTB = 0x01;
            }
            else{
                PORTB = PORTB << 1;
            }
            

            break;
        case Button:
                if(PORTB == 0x02 && (~PINA & 0x01) == 0x01){
                 score++;
                
                if(score == 0x06){
                LCD_DisplayString(1, "6");
                }
                else if(score == 0x07){
                LCD_DisplayString(1, "7");
                }
                else if(score == 0x08){
                LCD_DisplayString(1, "8");
                }
                else if(score == 0x09){
                LCD_DisplayString(1, "Yabba Dabba Doo!!!!");
                }
                }
            if(PORTB == 0x01 && (~PINA & 0x01) == 0x01 || PORTB == 0x04 && (~PINA & 0x01) == 0x01 ){
                score = score - 1;
                if(score == 0x05){
                LCD_DisplayString(1, "5");
                }
                 if(score == 0x06){
                LCD_DisplayString(1, "6");
                }
                 if(score == 0x07){
                LCD_DisplayString(1, "7");
                }
                 if(score == 0x08){
                LCD_DisplayString(1, "8");
                }
            }
        break;
        default:
        PORTB = 0x01;
            break;
    }
}
int main(void){
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x01;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    TimerSet(300);
    TimerOn();
    LCD_init();
    state = Start;
    // unsigned char tmpB = 0x00;

    while(1){
        // PORTB = 0x01;
        Tick();
        // tmpB = ~tmpB;
        // PORTB = tmpB;

        while(!TimerFlag){};
        TimerFlag = 0;
    }
}


