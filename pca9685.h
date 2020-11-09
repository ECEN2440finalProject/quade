/*
 * pca9685.h
 *
 *  Created on: Jun 22, 2020
 *      Author: Tyler Davidson
 */

#ifndef PCA9685_H_
#define PCA9685_H_

#include "i2c.h"

/*
 * In the PCA9685 datasheet, it is expected that
 * the device would be used to control LEDs. Therefore,
 * the registers shown in the datasheet are named
 * after which LED they belong to. For clarity, all LED labels
 * have been renamed as SERVO. This reinforces that this
 * driver is to be used with Servo Motors
 */

#define PRESCALER_50HZ  0x79

#define PCA_ADDRESS     0x5a

#define MAX_COUNT       4095
#define HIGH_MASK       0xf00
#define LOW_MASK        0x0ff







//Register Definitions
#define     PCA_MODE1             0x00
#define     PCA_MODE2             0x01
#define     PCA_SUBADR1           0x02
#define     PCA_SUBADR2           0x03
#define     PCA_SUBADR3           0x04
#define     PCA_ALLCALLADR        0x05

//SERVO0
#define     PCA_SERVO0_BASE         0x06
#define     PCA_SERVO0_ON_L         0x06
#define     PCA_SERVO0_ON_H         0x07
#define     PCA_SERVO0_OFF_L        0x08
#define     PCA_SERVO0_OFF_H        0x09

//SERVO1
#define     PCA_SERVO1_BASE          0x0a
#define     PCA_SERVO1_ON_L         0x0a
#define     PCA_SERVO1_ON_H         0x0b
#define     PCA_SERVO1_OFF_L        0x0c
#define     PCA_SERVO1_OFF_H        0x0d

//SERVO2
#define     PCA_SERVO2_BASE         0x0e
#define     PCA_SERVO2_ON_L         0x0e
#define     PCA_SERVO2_ON_H         0x0f
#define     PCA_SERVO2_OFF_L        0x10
#define     PCA_SERVO2_OFF_H        0x11

//SERVO3
#define     PCA_SERVO3_BASE         0x12
#define     PCA_SERVO3_ON_L         0x12
#define     PCA_SERVO3_ON_H         0x13
#define     PCA_SERVO3_OFF_L        0x14
#define     PCA_SERVO3_OFF_H        0x15

//SERVO4
#define     PCA_SERVO4_BASE         0x16
#define     PCA_SERVO4_ON_L         0x16
#define     PCA_SERVO4_ON_H         0x17
#define     PCA_SERVO4_OFF_L        0x18
#define     PCA_SERVO4_OFF_H        0x19

//SERVO5
#define     PCA_SERVO5_BASE          0x1a
#define     PCA_SERVO5_ON_L         0x1a
#define     PCA_SERVO5_ON_H         0x1b
#define     PCA_SERVO5_OFF_L        0x1c
#define     PCA_SERVO5_OFF_H        0x1d

//SERVO6
#define     PCA_SERVO6_BASE         0x1e
#define     PCA_SERVO6_ON_L         0x1e
#define     PCA_SERVO6_ON_H         0x1f
#define     PCA_SERVO6_OFF_L        0x20
#define     PCA_SERVO6_OFF_H        0x21

//SERVO7
#define     PCA_SERVO7_BASE         0x22
#define     PCA_SERVO7_ON_L         0x22
#define     PCA_SERVO7_ON_H         0x23
#define     PCA_SERVO7_OFF_L        0x24
#define     PCA_SERVO7_OFF_H        0x25

//SERVO8
#define     PCA_SERVO8_BASE         0x26
#define     PCA_SERVO8_ON_L         0x26
#define     PCA_SERVO8_ON_H         0x27
#define     PCA_SERVO8_OFF_L        0x28
#define     PCA_SERVO8_OFF_H        0x29

//SERVO9
#define     PCA_SERVO9_BASE         0x2a
#define     PCA_SERVO9_ON_L         0x2a
#define     PCA_SERVO9_ON_H         0x2b
#define     PCA_SERVO9_OFF_L        0x2c
#define     PCA_SERVO9_OFF_H        0x2d

//SERVO10
#define     PCA_SERVO10_BASE        0x2e
#define     PCA_SERVO10_ON_L        0x2e
#define     PCA_SERVO10_ON_H        0x2f
#define     PCA_SERVO10_OFF_L       0x30
#define     PCA_SERVO10_OFF_H       0x31

//SERVO11
#define     PCA_SERVO11_BASE        0x32
#define     PCA_SERVO11_ON_L        0x32
#define     PCA_SERVO11_ON_H        0x33
#define     PCA_SERVO11_OFF_L       0x34
#define     PCA_SERVO11_OFF_H       0x35

//SERVO12
#define     PCA_SERVO12_BASE        0x36
#define     PCA_SERVO12_ON_L        0x36
#define     PCA_SERVO12_ON_H        0x37
#define     PCA_SERVO12_OFF_L       0x38
#define     PCA_SERVO12_OFF_H       0x39

//SERVO13
#define     PCA_SERVO13_BASE        0x3a
#define     PCA_SERVO13_ON_L        0x3a
#define     PCA_SERVO13_ON_H        0x3b
#define     PCA_SERVO13_OFF_L       0x3c
#define     PCA_SERVO13_OFF_H       0x3d

//SERVO14
#define     PCA_SERVO14_BASE          0x3e
#define     PCA_SERVO14_ON_L        0x3e
#define     PCA_SERVO14_ON_H        0x3f
#define     PCA_SERVO14_OFF_L       0x40
#define     PCA_SERVO14_OFF_H       0x41

//SERVO15
#define     PCA_SERVO15_BASE        0x42
#define     PCA_SERVO15_ON_L        0x42
#define     PCA_SERVO15_ON_H        0x43
#define     PCA_SERVO15_OFF_L       0x44
#define     PCA_SERVO15_OFF_H       0x45

//ALL SERVO
#define     PCA_ALL_SERVO_BASE      0xfa
#define     PCA_ALL_SERVO_ON_L      0xfa
#define     PCA_ALL_SERVO_ON_H      0xfb
#define     PCA_ALL_SERVO_OFF_L     0xfc
#define     PCA_ALL_SERVO_OFF_H     0xfd

//PRE_SCALE
#define     PCA_PRE_SCALE           0xfe



//MODE1 Bits
//ALLCALL
#define     PCA_MODE1_ALLCALL_OFS       (0)
#define     PCA_MODE1_ALLCALL           ((uint8_t) 0x01)

//SUB3
#define     PCA_MODE1_SUB3_OFS          (1)
#define     PCA_MODE1_SUB3              ((uint8_t) 0x02)

//SUB2
#define     PCA_MODE1_SUB2_OFS          (2)
#define     PCA_MODE1_SUB2              ((uint8_t) 0x04)

//SUB1
#define     PCA_MODE1_SUB1_OFS          (3)
#define     PCA_MODE1_SUB1              ((uint8_t) 0x08)

//SLEEP
#define     PCA_MODE1_SLEEP_OFS         (4)
#define     PCA_MODE1_SLEEP             ((uint8_t) 0x10)

//AI
#define     PCA_MODE1_AI_OFS            (5)
#define     PCA_MODE1_AI                ((uint8_t) 0x20)

//EXTCLK
#define     PCA_MODE1_EXTCLK_OFS        (6)
#define     PCA_MODE1_EXTCLK            ((uint8_t) 0x40)

//RESTART
#define     PCA_MODE1_RESTART_OFS       (7)
#define     PCA_MODE1_RESTART           ((uint8_t) 0x80)


//MODE2 Bits
//OUTNE[1:0]
#define     PCA_MODE2_OUTNE_OFS         (0)
#define     PCA_MODE2_OUTNE_0           ((uint8_t) 0x01)
#define     PCA_MODE2_OUTNE_1           ((uint8_t) 0x02)

//OUTDRV
#define     PCA_MODE2_OUTDRV_OFS        (2)
#define     PCA_MODE2_OUTDRV            ((uint8_t) 0x04)

//OCH
#define     PCA_MODE2_OCH_OFS           (3)
#define     PCA_MODE2_OCH               ((uint8_t) 0x08)

//INVRT
#define     PCA_MODE2_INVRT_OFS         (4)
#define     PCA_MODE2_INVRT             ((uint8_t) 0x10)











void servo_write(uint8_t servo, uint8_t degrees);
void pca9685_init(void);













#endif /* PCA9685_H_ */
