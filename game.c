#include <game.h>

/*
 * game.c
 *
 *  Created on: Dec 13, 2021
 *      Author: Aidan Derocher
*/

QueueHandle_t Queue_Game;
TaskHandle_t Task_Game_Handle;
TaskHandle_t Task_Game_Timer_Handle;


struct node {
   int x;
   int y;
   struct node *next;
   struct node *prev;
   bool old; //records previous snake nodes to rewind
   JOYSTICK_DIR_t helddir;

};

volatile struct node *head;
struct node *middle;
volatile struct node *tail;
volatile bool gameover;
volatile bool fruit;
volatile int fruitx;
volatile int fruity;
volatile int oldheadx;
volatile int oldheady;

void Task_Game_Timer(void *pvParameters)
{
    while(1) //upper half of game logic
    {
        /*
         * Delay 200mS
         */
        vTaskDelay(pdMS_TO_TICKS(300));

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        vTaskNotifyGiveFromISR(Task_Game_Handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);



    }
}

void Task_Game(void *pvParameters) //lower half of game logic
{

    head = (struct node*) malloc(sizeof(struct node));
    middle = (struct node*) malloc(sizeof(struct node));
    tail = (struct node*) malloc(sizeof(struct node));
    gameover = false;
    fruit = false;
    fruitx = -1;
    fruity = -1;
    oldheadx = -1;
    oldheady = -1;

    srand(time(1)); //seeds our randomization

    //initializes snake in middle of screen, facing right, as a linked list
    head->x = 64;
    head->y = 64;
    head->next = middle;
    head->prev = NULL;
    head->old = false;
    head->helddir = JOYSTICK_DIR_RIGHT;

    middle->x = 60;
    middle->y = 64;
    middle->next = tail;
    middle->prev = head;
    middle->old = false;
    middle->helddir = JOYSTICK_DIR_RIGHT;

    tail->x = 56;
    tail->y = 64;
    tail->next = NULL;
    tail->prev = middle;
    tail->old = false;
    tail->helddir = JOYSTICK_DIR_RIGHT;

    bool fruitate = false;


    JOYSTICK_DIR_t dir = JOYSTICK_DIR_RIGHT;

    JOYSTICK_DIR_t newdir;

    while(1) {



        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);//wait for game clock

        if(gameover == true) { //rest
            struct node* temp = head;
            while(temp != NULL) {
                struct node* nexttemp = temp -> next;
                free(temp);
                temp = nexttemp;
            }

            head = (struct node*) malloc(sizeof(struct node));
            middle = (struct node*) malloc(sizeof(struct node));
            tail = (struct node*) malloc(sizeof(struct node));

            head->x = 56;
            head->y = 64;
            head->next = middle;
            head->prev = NULL;
            head->old = false;
            head->helddir = JOYSTICK_DIR_RIGHT;

            middle->x = 52;
            middle->y = 64;
            middle->next = tail;
            middle->prev = head;
            middle->old = false;
            middle->helddir = JOYSTICK_DIR_RIGHT;

            tail->x = 58;
            tail->y = 64;
            tail->next = NULL;
            tail->prev = middle;
            tail->old = false;
            tail->helddir = JOYSTICK_DIR_RIGHT;

            dir = JOYSTICK_DIR_RIGHT;

            fruit = false;
            fruitate = false;
            fruitx = -1;
            fruity = -1;
        }

        //fruit spawning
        if(fruit == false) {
            bool valid = false;
            while(!valid) { //randomizes until fruit doesnt overlap snake
                fruitx = rand() % 31;
                fruitx = fruitx * 4;
                fruity = rand() % 31;
                fruity = fruity * 4;
                valid = true;
                struct node* temp = head;
                while (temp != NULL){ //checks every snake square, including rewinds
                    if((fruitx == temp->x && fruity == temp->y)) {
                        valid = false;
                    }
                    temp = temp-> next;
                }
            }
            fruit = true; //indicate new fruit spawned in

        }

        if(!shook || tail -> next == NULL) {

            if (xQueueReceive(Queue_Game, &newdir, 5) == pdPASS) { //update direction of snake travel, makes sure there is queue to pull from

                //if no input (center), then old dir is held
                if (newdir == JOYSTICK_DIR_LEFT) {
                    dir = newdir;
                }
                else if (newdir == JOYSTICK_DIR_RIGHT) {
                    dir = newdir;
                }
                else if (newdir == JOYSTICK_DIR_UP) {
                    dir = newdir;
                }
                else if (newdir == JOYSTICK_DIR_DOWN) {
                    dir = newdir;
                }

            }

            //move snake in direction
            if(dir == JOYSTICK_DIR_LEFT) { //moves head one grid pos left
                struct node *newhead = (struct node*) malloc(sizeof(struct node));
                newhead->x = head->x-4;
                newhead->y = head->y;
                newhead->next = head;
                newhead->prev = NULL;
                head->prev = newhead;
                newhead->old = false;
                newhead->helddir = dir;

                head = newhead;
            }

            if(dir == JOYSTICK_DIR_RIGHT) { //moves head one grid pos right
                struct node *newhead = (struct node*) malloc(sizeof(struct node));
                newhead->x = head->x+4;
                newhead->y = head->y;
                newhead->next = head;
                newhead->prev = NULL;
                head->prev = newhead;
                newhead->old = false;
                newhead->helddir = dir;

                head = newhead;
            }

            if(dir == JOYSTICK_DIR_UP) { //moves head one grid pos up
                struct node *newhead = (struct node*) malloc(sizeof(struct node));
                newhead->x = head->x;
                newhead->y = head->y-4;
                newhead->next = head;
                newhead->prev = NULL;
                head->prev = newhead;
                newhead->old = false;
                newhead->helddir = dir;

                head = newhead;
            }

            if(dir == JOYSTICK_DIR_DOWN) { //moves head one grid pos down
                struct node *newhead = (struct node*) malloc(sizeof(struct node));
                newhead->x = head->x;
                newhead->y = head->y+4;
                newhead->next = head;
                newhead->prev = NULL;
                head->prev = newhead;
                newhead->old = false;
                newhead->helddir = dir;

                head = newhead;
            }



            if(!fruitate) {//no fruit was eaten
                tail->old = true; //indicates old tail no longer part of snake
                tail = tail->prev; //moves tail to valid loc
            }
            else { //fruit was eaten, allow tail to stretch by 1
                fruitate = false;
            }


            struct node* oldcountstart = tail;
            //keeps only 10 old moves for rewind
            int old;
            for (old = 0; old < 10; old++) {
                if (oldcountstart->next != NULL){
                    oldcountstart = oldcountstart->next;
                }
            }

            //free unused snake blocks
            struct node* oldtemp = oldcountstart-> next;
            while(oldtemp != NULL) {
                struct node* nexttemp = oldtemp -> next;
                free(oldtemp);
                oldtemp = nexttemp;
            }
            oldcountstart->next = NULL;

            //COLLISION LOGIC

            //checks if snakes head overlaps any of snakes body
            struct node * temp = head->next;
            while(temp != NULL){
                if((head->x == temp->x) && (head->y == temp->y) && temp->old == false) {
                    gameover = true;
                }
                temp = temp->next;
            }

            //checks snake is in bounds
            if (head->x < 0 || head->x > 127 || head->y < 0 || head->y > 127) {
                gameover = true;
            }

            if(head->x == fruitx && head->y == fruity) { //snake hits fruit
                fruitate = true;
                fruit = false;
            }
            //resets the head to not erase, as not rewound
            oldheadx = -1;
            oldheady = -1;
        }
        else { //rewind
            struct node * temp = head->next; //move head back 1
            oldheadx = head->x; //used to clear old head from drawing
            oldheady = head->y;
            free(head);
            head = temp;
            tail = tail -> next;
            tail -> old = false;
            dir = head -> helddir;
        }

        unsigned long int lux = opt3001_read_lux();
        if(shook) {
            snake_color = LCD_COLOR_GRAY;
        }
        else if (lux > 600) {
            snake_color = LCD_COLOR_WHITE;
        }
        else if(lux > 400) {
            snake_color = LCD_COLOR_BLUE;
        }
        else if(lux > 200) {
            snake_color = LCD_COLOR_GREEN;
        }
        else {
            snake_color = LCD_COLOR_ORANGE;
        }



        /*unsigned long int lux = opt3001_read_lux();
        char buffer[50];
        ltoa(lux,buffer, 10);
        printf("Light Level: ");
        printf(buffer);
        printf("\n\r");*/

        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        vTaskNotifyGiveFromISR(Task_Snake_Handle, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

        vTaskDelay(50);

    }
}
