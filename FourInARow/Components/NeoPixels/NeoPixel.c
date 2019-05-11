/*
 * NeoPixel.c
 *
 * Created: 2019-04-12 18:00:36
 *  Author: fe4242bo-s
 */ 

extern void Reset();
extern void Send_One();
extern void Send_Zero();

#include "../../Constants/GameStrings.h"

#include <math.h>

/** Struct to define the RGB values for a pixel. Used to enumerate through an array of pixels. */
typedef struct {
	int r;
	int g;
	int b;
} pixel_t;

pixel_t leds[16];

/** Resets all pixels to their default values. */
void clearPixels()
{
	for (int i = 0; i < 20; i++) {
		leds[i].r = 0;
		leds[i].g = 0;
		leds[i].b = 0;
	}
	
	for (int j = 0; j < 24 * 16; j++)
	{
		Send_Zero();
	}
	Reset();
}
int bitCompare;

/** Sets the specified pixel to the specified RGB values. */
void setPixel(int x, int r, int g, int b) {
	leds[x].r = r;
	leds[x].g = g;
	leds[x].b = b;
	
	for (int i = 0; i < 16; i++) {
		//Green
		int n = leds[i].g;
		int d = 0b10000000;
		for(int j = 0;j<8;j++){
			bitCompare = (n&d);
			if(bitCompare>0){
				Send_One();
			}
			else{
				Send_Zero();
			}
					
			d=d>>1;
		}
		
		//Red
		n = leds[i].r;
		d = 0b10000000;
				
		for(int j = 0;j<8;j++){
			bitCompare = (n&d);
			if(bitCompare>0){
				Send_One();
			}
			else{
				Send_Zero();
			}
					
			d=d>>1;
		}
		
		//BLUE
		n = leds[i].b;
		d = 0b10000000;
		for(int j = 0;j<8;j++){
			bitCompare = (n&d);
			if(bitCompare>0){
				Send_One();
			}
			else{
				Send_Zero();
			}
			
			d=d>>1;
		}
	}

	Reset();
}
