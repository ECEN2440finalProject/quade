/*
 * servo.h
 *
 *  Created on: Jul 14, 2020
 *      Author: Tyler Davidson
 *     Updated: Aug 13, 2020
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "pca9685.h"

//Servo Definitions
//-----Servo-----Channel
#define URL         0
#define LRL         1

#define URA         4
#define LRA         5

#define ULL         9
#define LLL         10

#define ULA         13
#define LLA         14

//URA-> Upper Right Arm
//LLL-> Lower Left Leg
//etc.

#define RA          0
#define LA          1
#define LL          2
#define RL          3

#define RA_START    115
#define RL_START    115
#define LA_START    -115
#define LL_START    -115


#define KEEP        9999    //number to represent keeping the same position for a dimension
#define A1          35      //length of femur in cm
#define A2          75      //length of tibia in cm

float               speed;  //overall speed

void servo_timer_init(void);
void set_pos(int limb, float x, float y, float z);
void wait_reach(int limb);
void wait_all_reach(void);
void servo_service(void);
void servo_startup(void);


#endif /* SERVO_H_ */
