/*
 * task_console.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Joe Krachey
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

/******************************************************************************
* Task used to print out messages to the console
******************************************************************************/
void Task_Game(void *pvParameters);
void Task_Game_Timer(void *pvParameters);




#endif /* GAME_H_ */
