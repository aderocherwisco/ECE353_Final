/*
 * task_snake.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Vansh Tyagi
 */

#include <main.h>

#define QUEUE_LEN  2

TaskHandle_t Task_Snake_Handle;
QueueHandle_t Queue_Snake;

volatile bool eaten = false;  // win status

uint8_t delayTimeMs = 25;   // default delay time
volatile uint16_t snake_color; // color of snake
uint16_t egg_color = LCD_COLOR_RED; // color of egg

struct node {
   int x;
   int y;
   struct node *next;
   struct node *prev;
   bool old; //records previous snake nodes to rewind
};

/******************************************************************************
 * This function will will draw the Snake game
 ******************************************************************************/
void draw_fruit(void)
{
    //Egg
    lcd_draw_image(fruitx + 2, fruity + 2, snakeWidthPixels, snakeHeightPixels,
                      snakeBitmaps, egg_color, LCD_COLOR_BLACK);
}

/******************************************************************************
 * This function will initialize Queue_Snake
 ******************************************************************************/
void Task_Snake_Init(void)
{
    Crystalfontz128x128_Init(); // initializes LED
}

void welcome_message(void)
{
    lcd_draw_rectangle(LCD_HORIZONTAL_MAX/2, LCD_VERTICAL_MAX/2, LCD_HORIZONTAL_MAX,
                               LCD_VERTICAL_MAX, LCD_COLOR_BLACK);
    lcd_draw_image(67, 67, welcomeWidthPixels, welcomeHeightPixels,
                   welcomeBitmaps, LCD_COLOR_GRAY, LCD_COLOR_BLACK);

    vTaskDelay(pdMS_TO_TICKS(2500));
    lcd_draw_rectangle(LCD_HORIZONTAL_MAX/2, LCD_VERTICAL_MAX/2, LCD_HORIZONTAL_MAX,
                               LCD_VERTICAL_MAX, LCD_COLOR_BLACK);

}

/******************************************************************************
 * This task manages the movement of the snake. The joystick task or the
 * console task can send messages to QUEUE_LEN
 ******************************************************************************/

void Task_Draw_Snake(void *pvParameters)
{

    uint8_t x = 67;
    uint8_t y = 67;

    uint8_t left_bound = 0;          // max left position
    uint8_t right_bound = 127;        // max right position
    uint8_t up_bound = 0;            // max up position
    uint8_t down_bound = 127;         // max down position
    welcome_message();

    while (1) {
        if(gameover) {
            welcome_message();
            gameover = false;
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //wait for game clock
        //clear screen

        if(oldheadx != -1) {
            lcd_draw_rectangle(oldheadx+2, oldheady+2, 4, 4, LCD_COLOR_BLACK); //clear for rewind
        }

        struct node* temp = head;
        while (temp != NULL) {
            if (temp->old == true) {


                lcd_draw_rectangle(temp -> x+2, temp -> y+2, 4, 4, LCD_COLOR_BLACK);
            }
            temp = temp -> next;

        }

        // Draw the snake to screen
        temp = head;
        lcd_draw_rectangle(temp -> x+2, temp -> y+2, 4, 4, LCD_COLOR_CYAN);
        temp = temp -> next;
        while (temp != NULL) {
            if (temp->old == false) {


                lcd_draw_rectangle(temp -> x+2, temp -> y+2, 4, 4, snake_color);
            }

            temp = temp -> next;
        }
        if (fruit) {
            draw_fruit();
        }

        //this is the final "game over" display once the snake dies
        if(gameover) {
            lcd_draw_rectangle(LCD_HORIZONTAL_MAX/2, LCD_VERTICAL_MAX/2, LCD_HORIZONTAL_MAX,
                                                   LCD_VERTICAL_MAX, LCD_COLOR_BLACK);
            lcd_draw_image(x, y, gameWidthPixels, gameHeightPixels, gameBitmaps,
                           LCD_COLOR_GRAY,
                               LCD_COLOR_BLACK);

            vTaskDelay(pdMS_TO_TICKS(2500));

            }
        }
}
