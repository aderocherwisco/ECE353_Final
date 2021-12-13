/*
 * task_maze.c
 *
 *  Created on: Dec 13, 2021
 *
 */

#include <main.h>

#define QUEUE_LEN  2

TaskHandle_t Task_Snanke_Handle;
QueueHandle_t Queue_Snake;

volatile bool eaten = false;  // win status

uint8_t delayTimeMs = 25;   // default delay time
uint16_t snake_color = LCD_COLOR_WHITE; // color of snake
uint16_t egg_color = LCD_COLOR_GRAY; // color of egg

/******************************************************************************
 * This function will will draw the Snake game
 ******************************************************************************/
void draw_game(void)
{
    //Egg
    lcd_draw_image(90, 30, snakeWidthPixels, snakeHeightPixels,
                      snakeBitmaps, LCD_COLOR_GRAY, LCD_COLOR_BLACK);
}

/******************************************************************************
 * This function will initialize Queue_Snake
 ******************************************************************************/
void Task_Snake_Init(void)
{
    Queue_Snake = xQueueCreate(QUEUE_LEN, sizeof(SNAKE_MSG_t)); // creates a queue snake
    Crystalfontz128x128_Init(); // initializes LED
}

void welcome_message(void)
{
    lcd_draw_image(67, 67, welcomeWidthPixels, welcomeHeightPixels,
                   welcomeBitmaps, LCD_COLOR_GRAY, LCD_COLOR_BLACK);

    vTaskDelay(pdMS_TO_TICKS(2500));
    Crystalfontz128x128_Init();
    draw_game();
}

/******************************************************************************
 * This task manages the movement of the logo. The joystick task or the
 * console task can send messages to QUEUE_LEN
 ******************************************************************************/

void Task_Snake(void *pvParameters)
{
    LOGO_MSG_t msg;
    uint8_t x = 67;
    uint8_t y = 67;

    uint8_t left_bound = 127;          // max left position
    uint8_t right_bound = 127;        // max right position
    uint8_t up_bound = 127;            // max up position
    uint8_t down_bound = 127;         // max down position

    welcome_message();

    // Draw the initial starting image of the snake
    lcd_draw_image(x, y, snakeWidthPixels, snakeHeightPixels, snakeBitmaps,
                   snake_color,
                   LCD_COLOR_BLACK);

    //Add the accelerometer and joystick logic here

    //this is the final game over display, we can add if statements around it

    lcd_draw_image(x, y, gameWidthPixels, gameHeightPixels, gameBitmaps,
                   LCD_COLOR_GRAY,
                       LCD_COLOR_BLACK);
}
