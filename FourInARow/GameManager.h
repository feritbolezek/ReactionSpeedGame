/*
 * GameManager.h
 *
 * Created: 2019-05-08 13:27:11
 *  Author: fe4242bo-s
 */ 


#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

 extern uint8_t difficulty;

 void InitializeGame();
 void ResetGame();
 void UpdatePoints();
 void EndGame();
 void difficultyDelay();
 void updatePixel();
 uint8_t diffRValue();
 uint8_t diffGValue();
#endif /* GAMEMANAGER_H_ */