/*	Author: Ian Oh
 *	Lab Section: 22
 *	Assignment: FINAL PROJECT 
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Video Link: https://youtu.be/Hq5EclrWsi0
 * Video Link: (Safety in case you want better lighting) https://youtu.be/OABdw8HsF7g
 * Google drive: https://drive.google.com/drive/folders/1dQdqkxcxxN-bBxIEy8wHV5cxIVQ1o0rJ?usp=sharing
 */

#include <avr/io.h>
#include "io.h"
#include <taskScheduler.h>
 #include <avr/interrupt.h>
 #include <avr/eeprom.h>
 #ifdef _SIMULATE_
 #include "simAVRHeader.h"
 #include "timer.h"

 #endif

unsigned short input = 0;
unsigned short moveCount = 0;
unsigned short enemyShift = 0;
unsigned short hold = 0x02;
unsigned char value = 0x00;
unsigned int playerScore = 0x00;
unsigned char playerString[3];
unsigned char address = 0x0F;
unsigned char numberOfShipsDestroyed = 0x00;
void A2D_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
//----------------------------
// Variables 
//----------------------------
char pattern[5] = {0xBF, 0x83, 0xFB, 0xE7, 0xEF};        //Title Screen
char pattern2[5] = {0xEF, 0xA1, 0x4F, 0x08, 0x04};	     //Title Screen 2
char fire_down[6] = {0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE}; //enemy shooting
char fire_up[6] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF};   //player shooting 
char game_over[5] = {0x0C, 0x83, 0xFB, 0xE7, 0xEF};			
char game_over2[5] = {0x3F, 0xA1, 0x4F, 0x08, 0x04};

unsigned char enemy = 0x3F;  //enemy 
unsigned char enemy2= 0xCE;  //enemy 2

unsigned char count, fire_count, player_shoot = 0;
unsigned int setFlag = 0x00;
unsigned char fireFlag = 0x00;
unsigned int explodeFlag = 0x00;
unsigned int lcd_flag = 0x00;
unsigned int goneFlag = 0x00;
unsigned char row = 0xFE;
unsigned char bitCount = 0x08;
//----------------------------
// FUNCTIONS
//----------------------------
void shiftRegister_C(unsigned char data){ // Shift register 
	for(unsigned char i = 0; i < 8; ++i){
		//SRCLR put on high 
		PORTC = 0x08;		

		PORTC = PORTC | ((data >> i) & 0x01);
		//SRCLCK or data line put on high 
		//SRCLCK or the Input clock is what shifts data into memory. 
		PORTC |= 0x04;
	}
	//RCLK Put on high 
	PORTC = PORTC | 0x02;
	//clear bits for next transmission 
	PORTC = 0x00;
}
void shiftRegister_D(unsigned char data){

	for(unsigned char i = 0; i < 8; ++i){
		PORTD = 0x08;

		//SRCLCK or the Input clock is what shifts data into memory. 
		PORTD |= ((data >> i) & 0x01);
		PORTD |= 0x04;
	}
	//Any time the output clock RCLCK is triggered, the data in the memory is copied to the output. 
	//Copies data from shift register to storage register 
	PORTD |= 0x02;
	PORTD = 0x00;
}
//----------------------------
// TASK 1
//----------------------------

unsigned char moveRow = 0xFE;
unsigned char moveCol = 0x08;
unsigned char temp = 0xFF, temp2 = 0xFE;
unsigned char button_A3, button_A2, button_A1, button_A0;

enum States{Wait, Move, Left, Right, Reset, Release}state;
int Tick(int state){
	input = ADC;
	switch(state){
		case Wait:
			if(button_A2){
				state = Move;
			}
			else if(button_A1)
				state = Right;
			else if(button_A0)
				state = Left;
			else if(button_A3)
				state = Reset;
			else
				state = Wait;
			break;
		case Move:
			if(button_A2){			
				state = Move;
			}
			else{
				++setFlag;
				if(setFlag >= 0x01){
					if(input < 0x1F4){
						++moveCount;
						state = Left;
					}
					else if( input > 0x212){
						++moveCount;
						state = Right;
					}
				}
			state = Release;
			}
			break;
		case Left:
			state = Release;
			break;
		case Right:
			state = Release;
			break;
		case Release:
			if(!button_A0 && !button_A1){
				state = Wait;
			}
			else{
				state = Release;
			}
			break;
		case Reset:
				state = Wait;
			break;
		default:
			state = Wait;
	 break;
	}
	switch(state){
			case Wait:
				if(setFlag >= 0x02){			//Player's character on the bottom of the screen 
					shiftRegister_C(moveRow);
					shiftRegister_D(moveCol);
				}
				if(numberOfShipsDestroyed == 0x02){ //All ships destroyed!
				shiftRegister_C(0x18); // The dot that will be unlit  1- STARTS at the bottom with the board face up
				shiftRegister_D(0xE7); 
				//You Win!
			}
			break;
		case Move: 
			if(lcd_flag == 0x00){				//LCD Displays onscreen directions 		
			LCD_ClearScreen();
			LCD_DisplayString(1, "Shoot the target ships to win!");
			lcd_flag = 0x01;
		}
		if(lcd_flag == 0x01){						//Adds 5 points for blowing up right ship 
			itoa(playerScore, playerString, 10);	//Player's score is converted to a string, 10 = decimal. 
			LCD_ClearScreen();
			LCD_DisplayString(1, "Player Score: ");
			LCD_DisplayString(18, playerString);
		}
		if(lcd_flag == 0x02){						//Adds 10 points for blowing up right ship 
			itoa(playerScore, playerString, 10);
			LCD_ClearScreen();
			LCD_DisplayString(1, "Player Score: ");
			LCD_DisplayString(18, playerString);
		}
			break;
			case Left:
				if(moveCol < 0x80){
					moveCol = (moveCol << 1);
				}
			break;
			case Right:
				if(moveCol > 0x01){
					moveCol = (moveCol >> 1);
				}
			break;
		case Release:
			if(!button_A0 && !button_A1) // No buttons are being held 
				state = Wait;
			else
				state = Release;
			break;
		case Reset:			//Player resets the game 
			setFlag 	= 0x00;
			fireFlag    = 0x00;
			explodeFlag = 0x00;
			lcd_flag 	= 0x00;
			goneFlag 	= 0x00;
			count 		= 0x00;
			fire_count 	= 0x00;
			player_shoot= 0x00;
			playerScore = 0x00;
			moveRow 	= 0xFE;
			moveCol 	= 0x08;
		break;
		default:
			state = Wait;
			break;
	}
	return state;
}
//----------------------------
// TASK 2
//----------------------------
enum DisplayState{display}dstate;
int D_Tick(int dstate){
	switch(dstate){
		case display:  
		if(setFlag == 0x00){
		dstate = display;
	}
		break;
		default:
		dstate = display;
		break;
	}
	switch(dstate){
		if(setFlag == 0x00){
			shiftRegister_C(0x2D); // The dot that will be unlit  1- STARTS at the bottom with the board face up
			shiftRegister_D(0xAD); //The row it will be in 1- STARTS  from the ledt 
			// PORTC = temp;
			// PORTD = row;
		}
			break;
		default:
			setFlag = 0x00;
			break;
	}
	return dstate;
}
//----------------------------
// TASK 3
//----------------------------
enum DisplayState2{display1}dstate1;
int E_Tick(int dstate1){
	switch(dstate1){
		case display1:
		if(setFlag == 0x00){
				dstate1 = display1;
		}
			break;
		default:
			dstate1 = display1;
	}
	switch(dstate1){
		case display1:
			if(setFlag == 0x00){
			shiftRegister_C(0x5A); // The dot that will be unlit  1- STARTS at the bottom with the board face up
			shiftRegister_D(0x52); //The row it will be in 1- STARTS  from the ledt 
		}
		break;
	}
	return dstate1;
}
//----------------------------
// TASK 4
//----------------------------
enum DisplayState4{display4} dstate4;
int F_Tick(int dstate4){
	switch(dstate4){
		case display4:
			if(numberOfShipsDestroyed == 0x02){
				lcd_flag == 0x03;
			}
			if(setFlag >= 0x02){
				if(explodeFlag == 0x00){ //No ships destroyed
				shiftRegister_C(enemy); // The dot that will be unlit  1- STARTS at the bottom with the board face up
				shiftRegister_D(enemy2); //The row it will be in 1- STARTS  from the ledt 
				}
				else if(explodeFlag == -2){ //Left Ship destroyed!
				shiftRegister_C(0x3F); // The dot that will be unlit  1- STARTS at the bottom with the board face up
				shiftRegister_D(0xC0);
				}
				else if(explodeFlag == -3){ //Right ship desroyed
				// 0x3f
				// 0xce
				shiftRegister_C(0x3F); // The dot that will be unlit  1- STARTS at the bottom with the board face up
				shiftRegister_D(0x00); 
				} 
			}
			if(setFlag == -1){			//GAME OVER - RESET 
				shiftRegister_C(0x7E);
				shiftRegister_D(0x81);
				setFlag = 0x00;
				fireFlag = 0x00;
				explodeFlag = 0x00;
				lcd_flag = 0x00;
				goneFlag = 0x00;
				count = 0x00;
				fire_count = 0x00;
				player_shoot = 0x00;
				playerScore = 0x00;
				numberOfShipsDestroyed = 0x00;
				moveRow = 0xFE;
				moveCol = 0x08;
			break;
		default:
			dstate4 = display4;
			break;
	} 
	return dstate4;
}
//----------------------------
// TASK 5
//----------------------------
enum DisplayState5{display5} dstate5;
int G_Tick(int dstate5){
	switch(dstate5){
		case display5:
		if(setFlag == 0x01){				//Start screen!(stars)
			shiftRegister_C(pattern[count]);
			shiftRegister_D(pattern2[count]);
					++count;

			if(count > 4){
				count = 0;
			}
		}
			break;
		default:
			dstate5 = display5;
	}
	return dstate5;
}
//----------------------------
// TASK 6
//----------------------------
enum DisplayState6{display6, player_fire} dstate6;
int FIRE_Tick(int dstate6){
		switch(dstate6){
		case display6:
			if(button_A2 || fireFlag == 1){ // Player fires a missile 
			dstate6 = player_fire;
			}
			else{
				dstate6 = display6;
			}
			break;
		default:
			dstate6 = display6;
	} 
	switch(dstate6){
		case display6:
		if(setFlag >= 0x02 && setFlag != -1){		//Enemy shoots missiles
			if(explodeFlag == 0){
			shiftRegister_C(fire_down[fire_count]);
			shiftRegister_D(0x44);
			++fire_count;
			if(fire_count > 6){
				fire_count = 0;
			}
		}
			if(explodeFlag == -2){					//Disables left ship fire when blown up
			shiftRegister_C(fire_down[fire_count]);
			shiftRegister_D(0x40);
			++fire_count;
			if(fire_count > 6){
				fire_count = 0;
			}
		}
			if(explodeFlag == -3){					//Disables right ship fire when blown up
			shiftRegister_C(fire_down[fire_count]);
			if(PORTD == 0x40){
			shiftRegister_D(0x00);
			} 
			else{
			shiftRegister_D(0x04);
			}
			++fire_count;
			if(fire_count > 6){
				fire_count = 0;
			}
		}
		}
			if(moveCol == 0x04){      //Game over- collision with player and object
				if(fire_count == 6){
					setFlag = -1;
				}
			}
			if(moveCol == 0x40){		//Game over- collision with player and object 
				if(fire_count == 6){
					setFlag = -1;
				}
			}
			break;
			case player_fire:
				fireFlag = 1;
				if(setFlag >= 0x02){		//Player fire 
					shiftRegister_C(fire_up[player_shoot]);
					shiftRegister_D(moveCol);
					++player_shoot;
					if(player_shoot > 6){
						fireFlag = 0;
						player_shoot = 0;
					}
				}
			if(moveCol == 0x08 && explodeFlag != -2 && numberOfShipsDestroyed <= 1){ //Destroy left ship (right hitbox)
				if(player_shoot == 6){
					playerScore += 5;
					lcd_flag = 0x01; 
					explodeFlag = -2;
					++numberOfShipsDestroyed;
				}
			}
			if(moveCol == 0x02 && explodeFlag != -2 && numberOfShipsDestroyed <= 1){ //Destroy left ship (left hitbox)
				if(player_shoot == 6){
					playerScore += 5;
					lcd_flag = 0x01; 
					explodeFlag = -2;
					++numberOfShipsDestroyed;

				}
			}
			if(moveCol == 0x20 && explodeFlag != -3 && numberOfShipsDestroyed <= 1){ //Destroy right ship (left hitbox)
				// if(fire_up[count] == 0xDF) //<============================================= TRY THIS INSTEAD
				if(fire_count == 6){
					lcd_flag = 0x02; 
					playerScore += 10;
					explodeFlag = -3;
					++numberOfShipsDestroyed;

				}
			}
			if(moveCol == 0x80&& explodeFlag != -3 && numberOfShipsDestroyed <= 1){ //Destroy right ship (right hitbox)
				if(player_shoot == 6){
					playerScore += 10;
					lcd_flag = 0x02; 
					explodeFlag = -3;
					++numberOfShipsDestroyed;

				}
			}
			break;
		default:
			dstate6 = display6;
	} 
	return dstate6;
}
//----------------------------
// TASK 7 - NOT USED
//----------------------------
enum DisplayState3{display2, startScreen_press, startScreen_release}dstate2;
int START_Tick(int dstate2){		//START SCREEN
	switch(dstate2){
		case display2:
		if(button_A0){			
			state = startScreen_press;
		}
		else{
			state = display2;
		}
			break;
		case startScreen_press:
		if(button_A0){
			state = startScreen_press;
		}
		else{
			state = startScreen_release;
		}
		break;
		default:
			dstate2 = display2;
		break;
	}
		switch(dstate2){
		case startScreen_release:
			PORTC = 0x00;
			PORTD = 0x00;
		break;
		default:
		break;
	}
	return dstate1;
}
int main(void) {
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    const char Init = -1;
	A2D_init();
	LCD_init();
	unsigned short highScore = eeprom_read_byte(address); 
	itoa(highScore, playerString, 10);
	LCD_DisplayString(3, "High Score:");
	LCD_DisplayString(19, playerString);
		if(playerScore > highScore){
			eeprom_write_byte(address, playerScore); 
		}
	
    static task task1, task2, task3, task4, task5, task6;
    task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = Init;
    task1.period = 5; //5
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick;

    task2.state = Init;
    task2.period = 900;
    task2.elapsedTime = task2.period;
    task2.TickFct = &D_Tick;

	task3.state = Init;
    task3.period = 1800;
    task3.elapsedTime = task3.period;
    task3.TickFct = &E_Tick;

	task4.state = Init;
    task4.period = 7; //7
    task4.elapsedTime = task4.period;
    task4.TickFct = &F_Tick;

	task5.state = Init;
    task5.period = 6; //6
    task5.elapsedTime = task5.period;
    task5.TickFct = &G_Tick;
	
	task6.state = Init;
    task6.period = 100;
    task6.elapsedTime = task6.period;
    task6.TickFct = &FIRE_Tick;
    TimerSet(1);
    TimerOn();
    unsigned short i;

    while (1) {
	    button_A0 = ~PINA & 0x01;
	    button_A1 = ~PINA & 0x02;
		button_A2 = ~PINA & 0x04;
		button_A3 = ~PINA & 0x08;
	    for(i = 0; i < numTasks; i++){
		    if(tasks[i]->elapsedTime == tasks[i]->period){
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			    tasks[i]->elapsedTime = 0;
		    }
	    tasks[i]->elapsedTime += 1;
    	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}