/*
 * Display.h
 *
 * Created: 2019-04-09 12:46:33
 *  Author: fe4242bo-s
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

// Comments for the subroutines can be found in their .c file.

extern void SendCommand(unsigned char cmd);
extern void SendChar(unsigned char data);
extern void SendString(unsigned char data[], int size);
extern void ClearDisplay();
	   void SkipToNextRow(int size, int atIndex);
	   void SendError();
	   void DisplayRoundTimer(char data[], int atIndex , int size); // Don't know the size of array, so we pass it around instead.
	   uint8_t ExtractEscapeCommand(char data[], int atIndex, int size);
extern void init_LED();
	   void EnableTimerInterrupt();
extern void EditLCDText(uint8_t index, char newData[], int size);



#endif /* DISPLAY_H_ */