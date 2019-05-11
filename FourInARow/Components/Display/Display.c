/*
 * Display.c
 *
 * Created: 2019-04-09 12:47:28
 *  Author: fe4242bo-s
 */ 

#define F_CPU 16000000

#define REGISTER_SELECT 7
#define READ_WRITE_SELECT 6
#define ENABLE_SELECT 1

#define SECOND_ROW_FIRST_COLUMN_LCD_ADDRESS 40
#define INCREASE_ARRAY_BY_GAME_TIME_BITS 2
#define DDRAM_ADDRESS_COMMAND 7
#define LCD_SECOND_ROW_FIRST_ADDRESS 40

// DDRD == Data port
// DDRC == Control port
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t newIndex = 0;

int timerCountActive = 0; // If it is active, the string currently shown should be updated.
int interruptsEnabled = 0;

int timerIndex = 0; // The index of where the last timer was shown
int timerTextSize = 0; // Size of the text that the timer is shown in
int timercount = 60;
int linebreakIndex = 0; // index where linebreak occurs

char timerCountStr[2];

/** Compare Match Timer interrupt used to get precise timings. This is used to send updates to the LCD at precisely at 1 update per second. */
ISR(TIMER1_COMPA_vect) {
	timercount--;
	if (timercount != 0) {
		sprintf(timerCountStr, "%d", timercount);
		EditLCDText(timerIndex,timerCountStr, 2);	
	}
	
	if (timercount == 10) {
		ClearDisplay();
		SendString("Yay!",4);
	}
}

/** Send command to the display (RS = 0) */
void SendCommand(char cmd) {
	PORTB = cmd;
	PORTC &= ~(1<<REGISTER_SELECT);
	PORTC &= ~(1<<READ_WRITE_SELECT);
	PORTC |= (1<<ENABLE_SELECT);
	_delay_us(1);
	PORTC &= ~(1<<ENABLE_SELECT);
	_delay_ms(1);
}

/** Send data to the display (RS = 1) */
void SendChar(unsigned char data) {
	PORTB = data;
	PORTC |= 1<<REGISTER_SELECT;
	PORTC &= ~(1<<READ_WRITE_SELECT);
	
	PORTC |= (1<<ENABLE_SELECT);
	_delay_us(1);
	PORTC &= ~(1<<ENABLE_SELECT);
	_delay_us(50);
	
}

// Hops to row 2.
void SkipToNextRow(int size, int atIndex) {
	SendCommand((1<<DDRAM_ADDRESS_COMMAND) + LCD_SECOND_ROW_FIRST_ADDRESS);
}

/** Displays the time for a round. */
void DisplayRoundTimer(char data[], int atIndex , int size) {
	
	timerIndex = atIndex;
	timerTextSize = size;
	data[atIndex] = '6';
	data[atIndex + 1] = '0';
	
	timerCountActive = 1;
}

/** Clears all data from the LCD DDRAM. */
ClearDisplay() {
	SendCommand(0b00000001); // TODO: Setup macros for binary commands.
}

/** Extracts the escape command and performs actions based on what command was sent in. Returns the next index of char to read once the action is done. */
uint8_t ExtractEscapeCommand(char data[], int atIndex, int size) {
	if (data[atIndex + 1] == 'n') {
		SkipToNextRow(size,atIndex);
		linebreakIndex = atIndex;
		return atIndex = atIndex + 2;
	}
	else if (data[atIndex + 1] == 't') {
		DisplayRoundTimer(data,atIndex,size);
		return atIndex - 1; // The escape character has been replaced by timer, write out new chars.
	}
	else { // if it isn't a letter then it's likely a number indicating the padding, NOTE: Padding should be a integer value between 0-9
		if (isdigit(data[atIndex + 1])) {
			// TODO: Program logic of padding.
		} else {
			
		}
	}
	return atIndex;
}

/** Sends a string to the LCD and displays it. */
void SendString(unsigned char data[], int size) {

	if (timerCountActive) { 
		 TCCR1B &= ~(1 << CS12);
		 timerCountActive = 0;
		 interruptsEnabled = 0; 
		 }
	
	for (int i = 0; i < size;i++)
	{
			if (data[i] == 'Q') {
				newIndex = ExtractEscapeCommand(data,i,size);
				i = newIndex;
				continue;
			}
			PORTB = data[i];
			PORTC |= 1<<REGISTER_SELECT;
			PORTC &= ~(1<<READ_WRITE_SELECT);
			
			PORTC |= (1<<ENABLE_SELECT);
			_delay_us(1);
			PORTC &= ~(1<<ENABLE_SELECT);
			_delay_ms(1);	
	}
	if (timerCountActive && !interruptsEnabled) {
		EnableTimerInterrupt();
	}
}

/** Changes a portion of the text displayed on the LCD at the specified index. */
void EditLCDText(uint8_t index, char newData[], int size) { // Second row is from 40 to 67
	
	if (index > 16) {
	SendCommand((1<<DDRAM_ADDRESS_COMMAND) + LCD_SECOND_ROW_FIRST_ADDRESS + (timerIndex - linebreakIndex) - 3); // -3 offset to skip escape chars.
	} else {
		// First row...
	}
	for (int i = 0; i < size; i++)
	{
		SendChar(newData[i]);
	}
}

/** Enables timer interrupts. */
EnableTimerInterrupt() {
	sei();
	TCCR1B |= 1 << WGM12; // CTC select
	TCCR1B |= 1 << CS12; // prescaler 256
	TIMSK1 |= 1 << OCIE1A; // the Timer/Counter1 Output Compare A Match interrupt is enabled
	
	OCR1A = (F_CPU/(1*2*256) - 1); // 0.48 is the time for calculation, calculated to achieve appx 1 interrupt per second. 16_000_000/(0.48*2*256) equal appx = 65_000/ 2^16 (16 bit timer)
	interruptsEnabled = 1;
}

/** Initialize the display */
void LCD_init(void) {
	DDRC = 0b11000010;
	DDRB = 0xFF;
	SendCommand(0b00111110); // Function set / 8 bit
	SendCommand(0b00001110); // Display on
	SendCommand(0b00000010); // Entry Mode // auto increase
	//SendCommand(0b00000010); //	return home
	SendCommand(0b00000001); // Clear display
}