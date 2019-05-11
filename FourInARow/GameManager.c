/*
 * GameManager.c
 *
 * Created: 2019-05-08 13:26:03
 *  Author: fe4242bo-s
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint-gcc.h>
#include <util/delay.h>
#include "Components/Display/Display.h"

uint32_t points = 0;
uint32_t highscore = 0;
		 difficulty = 1;

/** Initializes game and expects that display is already initialized. Make sure to initialize display before game initialization. */
 void InitializeGame() {
	points = 0;
	highscore = ReadFromEEPROM(0xFF);
	updateText();
 }
 
 /** Write data to permanent memory. */
 void WriteToEEPROM(unsigned int uiAddress, unsigned char ucData) {
	 while(EECR & (1<<EEPE));
	 
	 EEAR = uiAddress;
	 EEDR = ucData;
	 
	 EECR |= (1<<EEMPE);
	 EECR |= (1<<EEPE);
	 
 }
 
  /** Read data from permanent memory. */
 void ReadFromEEPROM(unsigned int uiAddress) {
	  while(EECR & (1<<EEPE));

	   EEAR = uiAddress;
	   EECR |= (1<<EERE);
	   return EEDR;
 }
 
 /** Will reset the game back to it's initial state. */
 void ResetGame() {
	points = 0;
	difficulty = 1;
	updateText();
 }
 
 /** Updates the points on the LCD display. Will increment points. */
 void UpdatePoints() {
	 
	points = (points + 1);
	if(points>highscore){ 
		highscore = points;
		WriteToEEPROM(0xFF,highscore);	
	}
	if(points%5==0 && difficulty != 7){difficulty++;}
	updateText();
 }
 
 /** Internal function that will do the updating of the text. */
 void updateText(){
	ClearDisplay();
	
	char txt09[1+9]="";
	char txt10[2+9]="";
	
	if (points<10){
		sprintf(txt09,"Score: %dQn",points);
		SendString(txt09,1+9);	
	}
	else{
		sprintf(txt10,"Score: %dQn",points);
		SendString(txt10,2+9);
	}
	
	txt09[1+11]="";
	txt10[2+11]="";
	
	if (highscore<10){
		sprintf(txt09,"Hi-Score: %d",highscore);
		SendString(txt09,1+10);
	}
	else{
		sprintf(txt10,"Hi-Score: %d",highscore);
		SendString(txt10,2+10);
	}
	
 }
 
 /** Delay that is based on difficulty. Meaning that the use of this during a neopixel animation will make the neopixels spin faster the harder it gets! */
 void difficultyDelay(){
	 for (int i = 8;i>difficulty;i--)
	 {
		 _delay_ms(40);
	 }
 }
 
 /** R portion of the RGB value based on difficulty. */
 uint8_t diffRValue(){
	 uint8_t r = 0;
	 for (int i = 0; i < difficulty; i++)
	 {
		 if ((r == 100)) return r;
		 r += 10;
	 }
	 return r;
 }
 
 /** G portion of the RGB value based on difficulty. */
 uint8_t diffGValue(){
	 uint8_t g = 80;
	 
	 for (int i = 0; i < difficulty; i++)
	 {
			 if ((g == 0)) return g;
		 	 g -= 10;
	 }
	 return g;
 }
 
 /** Ends the game. Updates High score and resets the game. */
 void EndGame() {
	 if(points>highscore){
		  highscore = points;
		  }
	 ResetGame();
 }