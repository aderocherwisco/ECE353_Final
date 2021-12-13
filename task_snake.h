/*
 * task_snake.h
 *
 *  Created on: Dec 13, 2021
 *
 */

#ifndef TASK_SNAKE_H_
#define TASK_SNAKE_H_

#include <main.h>

extern TaskHandle_t Task_Snake_Handle;
extern QueueHandle_t Queue_Snake;

typedef enum
{
    SNAKE_CMD_LEFT, SNAKE_CMD_RIGHT, SNAKE_CMD_UP, SNAKE_CMD_DOWN, SNAKE_CMD_CENTER,
} SNAKE_CMD_t;


typedef struct
{
    SNAKE_CMD_t cmd;
    uint8_t value;
} SNAKE_MSG_t;

extern volatile bool eaten;

/******************************************************************************
 * This function will initialize Queue_Snake
 ******************************************************************************/
void Task_Snake_Init(void);

/******************************************************************************
 * This task manages the movement of the maze. The joystick task or the
 * console task can send messages to the
 ******************************************************************************/
void Task_Snake(void *pvParameters);

/******************************************************************************
 * This function will will draw the Snake game
 ******************************************************************************/
void draw_game(void);

#endif /* TASK_SNAKE_H_ */
