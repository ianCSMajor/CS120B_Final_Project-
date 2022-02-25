/*  Author: Ian Oh
 *  Partner(s) Name: 
 *  Lab Section: 22
 *  Assignment: Lab # 10 Exercise # 1
 *  Exercise Description: [optional - include for your own benefit]
 *  Demo Link: https://youtu.be/DJipx5aRTHo
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 */
#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
volatile unsigned char TimerFlag = 0;
static unsigned char combineVar;
static unsigned char tempA;
static unsigned char inc;
static unsigned char blinkingLED;
static unsigned char aud;
unsigned char array[] = {1, 2, 3, 4, 5, 6};
static unsigned char j = 0x01;

void TimerISR(){
    TimerFlag = 1;
}
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
enum TL_states {START, BL1, BL2, BL3} TL_state;
enum BL_states {START1, BLINK, BLINK_OFF} BL_state;
enum CB_states {INIT, COMBINE} CB_state;
enum AUD_states {ON, OFF} AUD_state;
enum BTN_states {INIT1, AUDIO, DOWN, UP, RELEASE} BTN_state;
void TickFct_ThreeLEDsSM(){
switch(TL_state){
    case START: 
        TL_state = BL1;
        break;
    case BL1:
        TL_state = BL2;
        break;
    case BL2:
        TL_state = BL3;
        break;
    default:
        TL_state = BL1;
    break;
}
switch(TL_state){
    case BL1:
        tempA = 0x01;
        break;
    case BL2:
        tempA = 0x02;
        break;
    case BL3:
        tempA = 0x04;
        break;
    default:
        tempA = 0x00;
    break;
}
// PORTB = tempA;
}
void TickFct_BlinkingLEDSM(){
// static unsigned char blinkingLED;
switch(BL_state){
    case START1:
        BL_state = BLINK_OFF;
        break;
    case BLINK_OFF:
        BL_state = BLINK;
        break;
    case BLINK:
        BL_state = BLINK_OFF;
        break;
    default:
        BL_state = BLINK_OFF;
        break;
}
switch(BL_state){
     case BLINK_OFF:
        blinkingLED = 0x00;
        break;
    case BLINK:
        blinkingLED = 0x01;
        break;
    default:
        blinkingLED = 0x00;
        break;
}
// PORTB = blinkingLED;
}
void TickFct_Combine(){
switch(CB_state){
    case INIT:
        CB_state = COMBINE;
        break;
    case COMBINE:
        CB_state = COMBINE;
        break;
    default:
    CB_state = INIT;
    break;
}
switch(CB_state){
    case COMBINE:
    combineVar = ((aud << 4) | (tempA << 1) | blinkingLED);
    break;
    default:
    combineVar = 0x00;
    break;
}
PORTB = combineVar;
}
void TickFct_AUD(){
switch(AUD_state){
    case OFF:
    if((~PINA & 0x01) == 0x01){
        AUD_state = ON;
    }
    else{
        AUD_state = OFF;
    }
    break;
    case ON:
    if((~PINA & 0x01) == 0x01){
        AUD_state = ON;
    }
    else{
        AUD_state = OFF;
    }
    break; 
}
switch(AUD_state){
    case OFF:
        aud = 0x00;
        inc = 0;
        break;
    case ON:
     if(inc <= 2){
             aud = 0x01;
    }
     else if(inc <= 4){
         PORTB = 0x00;
         aud = 0x00;
     }
    else{
        inc = 0;
    }
    inc++;

     break;
     default:
     inc = 0;
    break;
}
}
void TickFct_Button(){

    switch(BTN_state){
        case INIT1:
        if((~PINA & 0x01) == 0x01){
            BTN_state = AUDIO; 
        }
        if((~PINA & 0x02) == 0x02){
            BTN_state = DOWN; 
        }
        if((~PINA & 0x04) == 0x04){
            BTN_state = UP; 
        }
        break;
        case AUDIO: 
        if((~PINA & 0x01) == 0x01){
            BTN_state = AUDIO; 
        }
        else{
        BTN_state = RELEASE;
        }
        break;
        case DOWN:
         if((~PINA & 0x02) == 0x02){
            BTN_state = DOWN; 
        }
        else{
            if(j > 0){
                --j;
            }
        BTN_state = RELEASE;
        }
        break;
         case UP:
          if((~PINA & 0x04) == 0x04){
            BTN_state = UP; 
        }
        else{
            if(j < 6){
                ++j;
            }
        BTN_state = RELEASE;
        }
        break;
        case RELEASE:
            if((~PINA & 0x07) == 0x00){
                BTN_state = INIT1;
            }
            else{
                BTN_state = RELEASE;
            }
    default:
    BTN_state = INIT1;
    break;
    }
    switch(BTN_state){
        case INIT1:
         aud = 0x00;
        inc = 0;
        break;
      case AUDIO:
            if(inc <= array[j]){
             aud = 0x01;
    }
     else if(inc <= (array[j] * 2)){
        //  PORTB = 0x00;
         aud = 0x00;
     }
    else{
        inc = 0;
    }
    ++inc;

        break;
    }
}

int main(void){
    unsigned long TL_elapsedTime = 300;
    unsigned long BL_elapsedTime = 1000;
    // unsigned long AUD_elapsedTime = 200;
    const unsigned long timerPeriod = 1;
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRB = 0xFF;
    PORTB = 0x00;
    TimerSet(timerPeriod);
    TimerOn();
    while(1){
        if(BL_elapsedTime >= 1000){
             TickFct_BlinkingLEDSM();  
             BL_elapsedTime = 0;  

        }
        if(TL_elapsedTime >= 300){
        TickFct_ThreeLEDsSM();
        TL_elapsedTime = 0;
        }
            // TickFct_AUD();
            // TickFct_AUD();
             TickFct_Button();

        TickFct_Combine();
        while(!TimerFlag){}
        TimerFlag = 0;
        TL_elapsedTime += timerPeriod;
        BL_elapsedTime += timerPeriod;
    }
    return 1;
}