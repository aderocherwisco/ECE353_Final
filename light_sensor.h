/*
 * opt3001.h
 *
 *  Created on: Dec 12, 2021
 *      Author: Aidan Derocher
*/

#ifndef OPT3001_H_
#define OPT3001_H_

#include "msp.h"
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>

#define I2C_LIGHT_ADDR   0x44 //slave address

//light sensor constants
#define LIGHT_OPT_INTERRUPT_PIN 11
#define LIGHT_RESULT_REG 0x00
#define LIGHT_CONFIG_REG 0x01
#define LIGHT_LOWLIMIT_REG 0x02
#define LIGHT_HIGHLIMIT_REG 0x03
#define LIGHT_MANUFACTUREID_REG 0x7E
#define LIGHT_DEVICEID_REG 0x7F

#define LIGHT_DEFAULT_CONFIG 0xCC10 // 800ms
#define LIGHT_DEFAULT_CONFIG_100 0xC410 // 100ms



/******************************************************************************
 * Initialize the opt3001 light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void);

/******************************************************************************
 * Returns the current light level in LUX units.
 ******************************************************************************/
unsigned long int opt3001_read_lux(void);

#endif /* OPT3001_H_ */
