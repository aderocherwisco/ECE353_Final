/*
 * task_console.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Joe Krachey
 */

#ifndef GAME_H_
#define GAME_H_

#include <main.h>

extern QueueHandle_t Queue_Game;
extern TaskHandle_t  Task_Game_Handle;

/******************************************************************************
* Task used to print out messages to the console
******************************************************************************/
void Task_Game(void *pvParameters);




#endif /* GAME_H_ */
