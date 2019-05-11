/*
 * FourInARow.c
 *
 * Created: 2019-04-09 12:44:38
 * Author : fe4242bo-s
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Components/Display/Display.h"
#include "Components/NeoPixels/NeoPixel.h"
#include "GameManager.h"
//#include "Constants/GameStrings.h"

/*
NOTES TO PROGRAMMER!

 - Hardcoded literals can be found in Constants/...
 - When passing a char array into Display with SendString(...) you can add Q{command} for special operations.
 - Some of these special operations are: Qn (jump to new line on LCD), Qt (display countdown timer on LCD), #CONSTANT padding CONSTANT amount in text.
 - Maximum #n command is ONCE (obviously)
 - Maximum #t command is ONCE Do not support more than 1 timer at the moment. (Never will, hardware limitations do not allow it :) )
 - Game buttons connected to --> 14,15,16,17
*/

extern void Reset();
extern void Send_One();
extern void Send_Zero();


int pressed = 0;
int currentLight = 0;
ISR(PCINT0_vect) {

	clearPixels();
	setPixel(currentLight,10,10,10);


	if (PINA & (1 << PINA0)) {
		 if (pressed) {
			 pressed = 0;
		 } 
		 else {
			 pressed = 1;
			 if(currentLight == 14) {
				UpdatePoints();
			 } else {
				EndGame();
			 }
			 
		 }
	}
	PCIFR |= PCIF0; // Do NOT remove! Will make sure we trigger button interrupt only once!
}


int main(void)
{
	cli();
	LCD_init();
	InitializeGame();
	
	PCICR |= 1 << PCIE0;
	sei();
	PCMSK0 |= 1 << PCINT0;
	
	// NEOPIXELS AFTER THIS

	DDRD = 1 << PORTD7;
	PORTD &= ~(1 << PORTD7);
		while (1) 
		{
			
			for(int i = currentLight; i<16 ;i++){
				if (pressed) break;
				currentLight = i;
				setPixel(i, diffRValue(),diffGValue(),0);
				difficultyDelay();
				setPixel(i,0,0,0);
				if (pressed) setPixel(i,10,10,10); // Fixes the case where the pixel is set to zero if a button press is faster than the diffDelay.
			}
			if (!pressed) currentLight = 0;
		}
	
}

