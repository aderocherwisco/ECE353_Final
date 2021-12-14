/*
 * game.h
 *
 *  Created on: Dec 13, 2021
 *      Author: Aidan Derocher
*/

#ifndef GAME_H_
#define GAME_H_

#include <main.h>
#include <time.h>

extern QueueHandle_t Queue_Game;
extern TaskHandle_t Task_Game_Timer_Handle;
extern TaskHandle_t  Task_Game_Handle;

extern volatile struct node* head;
extern volatile struct node* tail;
extern volatile bool gameover;
extern volatile bool fruit;
extern volatile int fruitx;
extern volatile int fruity;
extern volatile int oldheadx;
extern volatile int oldheady;

/******************************************************************************
* Tasks used to run main game logic
******************************************************************************/
void Task_Game(void *pvParameters);
void Task_Game_Timer(void *pvParameters);




#endif /* GAME_H_ */
