/*
 * Buzzer.h
 *
 *  Created on: Dec 13, 2021
 *      Author: sstaggs
 */
#ifndef BUZZER_H_
#define BUZZER_H_

#include "interrupts.h"
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>

#define NOTE_C3 131
#define NOTE_D3 147
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494

#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

void init_buzzer(uint16_t ticks_period);

void Buzzer_On(void);

void Buzzer_Off(void);

void changeTone(uint16_t ticks_period);

bool Buzzer_Status(void);

#endif /* BUZZER_H_ */
