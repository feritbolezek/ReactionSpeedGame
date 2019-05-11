/*
 * GameStrings.h
 *
 * Created: 2019-04-10 15:07:37
 *  Author: fe4242bo-s
 */ 

/** This file will only consist of hardcoded strings.  */

#ifndef GAMESTRINGS_H_
#define GAMESTRINGS_H_
// if \n calculate for new line, if \CONSTANT add padding. {xx} is the time.
extern const char START_TEXT[29] = "4 in a row\n Press 1 to start";
extern const char PLAYER_ONE_SETUP[23] = "Player 1\n Choose color";
extern const char PLAYER_TWO_SETUP[23] = "Player 2\n Choose color";
extern const char P1_CHOICE[] = "P1 Choose column \n {xx}";
extern const char P2_CHOICE[] = "P2 Choose column \n {xx}";
	
extern const char P1_WON[] = "Player 1 WON! \n {xx}";
extern const char P2_WON[] = "Player 2 WON! \n {xx}";
extern const char GAME_TIE[] = "TIE! \n {xx}";

extern const char GAME_PAUSE[] = "Game Paused \n Press 1 to start";
extern const int nPixels = 16;

#endif /* GAMESTRINGS_H_ */