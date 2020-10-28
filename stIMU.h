/*
 * stIMU.h
 *
 *  Created on: Aug 2, 2020
 *      Author: Ariel's Yoga
 *       https://github.com/STMicroelectronics/STM32CubeL4/blob/master/Drivers/BSP/Components/lis3mdl/lis3mdl.h
 *    Modified: Oct 2, 2020
 * Modified by: Arielle Blum
 */

#ifndef STIMU_H_
#define STIMU_H_

#include "msp.h"
#include "math.h"
#include "stdio.h"
#include "stdint.h"
#include "i2c.h"

#define CTR_REG1_SET                    0x3E //0b 1111 1111
#define CTR_REG2_SET                    0x60 //0b 0000 0000
#define CTR_REG3_SET                    0x00 //0b 0000 0000
#define CTR_REG4_SET                    0x0C //0b 0000 1100
#define CTR_REG5_SET                    0x40 //0b 0000 0000


/************** I2C Address *****************/

#define LIS3MDL_MAG_I2C_ADDRESS_LOW     0x38    // SAD[1] = 0
#define LIS3MDL_MAG_I2C_ADDRESS_HIGH    0x3C    // SAD[1] = 1

#define LIS3MDL_MAG_I2C_ADDRESS         0x1C

/************** Who am I  *******************/

#define LIS3MDL_MAG_WHO_AM_I            0x3D

/************** Device Register  *******************/
#define LIS3MDL_MAG_WHO_AM_I_REG        0X0F
#define LIS3MDL_MAG_CTRL_REG1           0X20
#define LIS3MDL_MAG_CTRL_REG2           0X21
#define LIS3MDL_MAG_CTRL_REG3           0X22
#define LIS3MDL_MAG_CTRL_REG4           0X23
#define LIS3MDL_MAG_CTRL_REG5           0X24
#define LIS3MDL_MAG_STATUS_REG          0X27
#define LIS3MDL_MAG_OUTX_L              0X28
#define LIS3MDL_MAG_OUTX_H              0X29
#define LIS3MDL_MAG_OUTY_L              0X2A
#define LIS3MDL_MAG_OUTY_H              0X2B
#define LIS3MDL_MAG_OUTZ_L              0X2C
#define LIS3MDL_MAG_OUTZ_H              0X2D
#define LIS3MDL_MAG_TEMP_OUT_L          0X2E
#define LIS3MDL_MAG_TEMP_OUT_H          0X2F
#define LIS3MDL_MAG_INT_CFG             0X30
#define LIS3MDL_MAG_INT_SRC             0X31
#define LIS3MDL_MAG_INT_THS_L           0X32
#define LIS3MDL_MAG_INT_THS_H           0X33

#define LIS3MDL_MAG_XDA                 ((uint8_t)0x01)
#define LIS3MDL_MAG_YDA                 ((uint8_t)0x02)
#define LIS3MDL_MAG_ZDA                 ((uint8_t)0x04)
#define LIS3MDL_MAG_ZYXDA               ((uint8_t)0x08)
#define LIS3MDL_MAG_XOR                 ((uint8_t)0x10)
#define LIS3MDL_MAG_YOR                 ((uint8_t)0x20)
#define LIS3MDL_MAG_ZOR                 ((uint8_t)0x40)
#define LIS3MDL_MAG_ZYXOR               ((uint8_t)0x80)





void config_LIS3MDL(void);

int16_t read_magnetometer_x(void);
int16_t read_magnetometer_y(void);
int16_t read_magnetometer_z(void);

void config_LSM6DSOX(void);

uint16_t read_acceleration_x(void);
uint16_t read_acceleration_y(void);
uint16_t read_acceleration_z(void);

uint16_t read_gyroscope_x(void);
uint16_t read_gyroscope_y(void);
uint16_t read_gyroscope_z(void);

double roll(void);
double pitch(void);
double yaw(void);


#endif /* STIMU_H_ */
