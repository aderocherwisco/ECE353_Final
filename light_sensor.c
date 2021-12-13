/*
 * opt3001.c
 *
 *  Created on: Dec 12, 2021
 *      Author: Aidan Derocher
 */

#include "light_sensor.h"

/******************************************************************************
 * Initialize the opt3001 light sensor on the MKII.  This function assumes
 * that the I2C interface has already been configured to operate at 100KHz.
 ******************************************************************************/
void opt3001_init(void)
{

    //configs to poll every 100 ms
    i2c_write_16(I2C_LIGHT_ADDR, LIGHT_CONFIG_REG, LIGHT_DEFAULT_CONFIG_100);


}

/******************************************************************************
 * Returns the current light level in lux.
 ******************************************************************************/
unsigned long int opt3001_read_lux(void)
{
    int16_t read_data; //holds raw read data

    // Reads the raw data from light sensor
    read_data = i2c_read_16(I2C_LIGHT_ADDR, LIGHT_RESULT_REG);

    //converts to lux levels
    uint16_t exp;
    uint32_t lux;

    lux = (read_data & 0x0FFF); //data portion of raw reading
    exp = ((read_data>>12) & 0x000F); //amount to shift lux by

    //multiplies data value based on exponent
    switch(exp){
        case 0: //*0.015625
            lux = lux>>6;
            break;
        case 1: //*0.03125
            lux = lux>>5;
            break;
        case 2: //*0.0625
            lux = lux>>4;
            break;
        case 3: //*0.125
            lux = lux>>3;
            break;
        case 4: //*0.25
            lux = lux>>2;
            break;
        case 5: //*0.5
            lux = lux>>1;
            break;
        case 6:
            lux = lux;
            break;
        case 7: //*2
            lux = lux<<1;
            break;
        case 8: //*4
            lux = lux<<2;
            break;
        case 9: //*8
            lux = lux<<3;
            break;
        case 10: //*16
            lux = lux<<4;
            break;
        case 11: //*32
            lux = lux<<5;
            break;
        }

    // Return the data in lux
    return lux;
}
