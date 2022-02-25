/*	Author: Ian Oh
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab # 9 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 * Demo Link: https://www.youtube.com/watch?v=4a78NHqt3PU
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
            state = ON_OFF;
        }
        else if((~PINA & 0x02) == 0x02){
            state = UP;
        }
        else if((~PINA & 0x04) == 0x04){
            state = DOWN;
        }
        else{
            state = INIT;
        }
    break;
    case ON_OFF:
        if((~PINA & 0x01) == 0x01){
            state = ON_OFF;
        } 
        else{

            state = INIT;
        }
        break;
     case UP:
        if((~PINA & 0x02) == 0x02){
            state = UP;
        } 
        else{
            if(count <= 8){
            count = count + 1;
            }

            state = OFF;
        }
        break;
        case UP_PRESS:
        state = UP;
        break;
     case DOWN:
        if((~PINA & 0x04) == 0x04){
            state = DOWN;
        } 
        else{
            if(count >= 1){
            count = count - 1;
            }
            state = OFF;
        }
        break;
    case DOWN_PRESS:
        state = DOWN;
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
    case ON_OFF:
        if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
        if((~PINA & 0x01) == 0x01){
            if(flag == 0x00){
            flag = 1;
              set_PWM(0);

            PWM_off();
            }
            else if(flag == 0x01){
                flag = 0;
                PWM_on();
                set_PWM(100);
            }
        } 
        break;
        case UP:
         if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
          if((~PINA & 0x02) == 0x02){
            if(count == 0x01){
            set_PWM(261.63);
            }
            else if(count == 0x02){
            set_PWM(293.66);
            }
            else if(count == 0x03){
                set_PWM(329.63);
            }
            else if(count == 0x04){
                set_PWM(349.23);
            }
            else if(count == 0x05){
                set_PWM(392.00);
            }
            else if(count == 0x06){
                set_PWM(440.00);
            }
            else if(count == 0x07){
                set_PWM(493.88);
            }
            else if(count == 0x08){
                set_PWM(523.25);
            }
      }
        break;
     case UP_PRESS:
        break;
     case DOWN:
      if((~PINA & 0x03) == 0x03 || (~PINA & 0x07) == 0x07 || (~PINA & 0x06) == 0x06){
            set_PWM(0);
        }
        if((~PINA & 0x04) == 0x04){
            if(count == 0x01){
            set_PWM(261.63);
            }
            else if(count == 0x02){
            set_PWM(293.66);
            }
            else if(count == 0x03){
                set_PWM(329.63);
            }
            else if(count == 0x04){
                set_PWM(349.23);
            }
            else if(count == 0x05){
                set_PWM(392.00);
            }
            else if(count == 0x06){
                set_PWM(440.00);
            }
            else if(count == 0x07){
                set_PWM(493.88);
            }
            else if(count == 0x08){
                set_PWM(523.25);
            }
        break;
    case OFF:
        set_PWM(0);
        default:
        break;

}
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