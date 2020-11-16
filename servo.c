/*
 * servo.c
 *
 * This driver is designed to give xyz control over all the limbs.
 * This will make it much easier to program movement for any kind of gait
 * because the angles can be disregarded. Additional functionality include
 * speed control of the limbs as well as moving several limbs simultaneously
 * thanks to a TimerA interrupt-driven scheduler.
 *
 * Much of this code is inspired by other work.
 * Many of the main functions are built from the work of
 * Belkhir Mohamed. Below is the information that he has requested we provide.
 *
 *************************************************************************************************************************************************************************
 * - Author               : BELKHIR Mohamed                        *
 * - Profession           : (Electrical Ingineer) MEGA DAS owner   *
 * - Main purpose         : Industrial Application                 *
 * - Copyright (c) holder : All rights reserved                    *
 * - License              : BSD 2-Clause License                   *
 * - Date                 : 20/04/2017                             *
 * ***********************************************************************************************************************************************************************
*********************************** NOTE **************************************\\
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED
 *
 *
 *
 *
 *
 *  Created on: Jul 14, 2020
 *      Author: Tyler Davidson
 *     Updated: Aug 13, 2020
 */
#include "servo.h"
#include "math.h"

void xyz_to_angle(int limb, volatile float *t1, volatile float *t2, volatile float x, volatile float y, volatile float z);
void servo_service(void);
void angle_to_servo(int limb, float t1, float t2);

//flag to indicate the need to service the servos
volatile bool timer_flag = false;

//matrix to organize all servos in the system
const int Servo[4][2] = { {URA,LRA} , {ULA,LLA} , {ULL,LLL} , {URL,LRL} };
//    Servo Matrix
//  __|_0_|_1_|_2_|_3_
//  0_|URA|ULA|URL|ULL
//  1_|LRA|LLA|LRL|LLL


volatile float  pos_now[4][3];         //current coords of each foot (xyz)
volatile float  pos_expected[4][3];    //expected coords of each foot
float           temp_speed[4][3];      //Incremental value to control speed of movement

/*
 * This function is designed to set the starting position for all
 * the limbs and update the arrays for first time use
 */
void servo_startup(){

    int i;
    int j;

    for(i=0; i<4; i++){
        for(j=0; j<2; j++){
            servo_write(Servo[i][j], 90);
        }
    }


    //updates array to represent the correct location
    for(i=0; i<4; i++){
        for(j=0; j<3; j++){
            pos_now[i][j] = 0;
        }
    }


    pos_now[0][0] = A1+A2;
    pos_now[1][0] = -(A1+A2);
    pos_now[2][0] = -(A1+A2);
    pos_now[3][0] = A1+A2;



}


/*
 * Sets up TimerA0 to request servicing of the servos every 20ms.
 * Uses SMCLK @ 3MHz
 */
void servo_timer_init(void){
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // TACCR0 interrupt enabled
    TIMER_A0->CCR[0] = 60000;               //count to 60,000 at 3MHz (T = 20ms)
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | // SMCLK , UP mode
            TIMER_A_CTL_MC__UP;
    NVIC_EnableIRQ(TA0_0_IRQn);


}


/*
 * This function checks each limb and moves them by an incrimental amount
 * based on the speed established inside the set_pos function. This function calls two very
 * large functions and requires a lot of time to finish completely. The length of execution
 * is related to the speed established.
 *
 * This function clears t he timer_flag upon completion.
 */
void servo_service(void){
//    P1->OUT |= BIT0;

    static float t1, t2;

    int i;
    int j;
    for(i = 0; i<4; i++){
        for(j = 0; j<3; j++){
            if(abs(pos_now[i][j] - pos_expected[i][j]) >= abs(temp_speed[i][j]))
                pos_now[i][j] += temp_speed[i][j];
            else
                pos_now[i][j] = pos_expected[i][j];
        }
        xyz_to_angle(i, &t1, &t2, pos_now[i][0],pos_now[i][1],pos_now[i][2]);
        angle_to_servo(i,t1,t2);
    }

//    P1->OUT &= ~BIT0;
    timer_flag = false; //lower the flag, servos were updated


}

/*
 * Sets a point for the selected leg to move towards.
 * Use "KEEP" to keep that dimension the same.
 *
 * Breaks up the total distance into several smaller steps along the way.
 * The number of steps is determined by the "speed" variable.
 */
void set_pos(int limb, float x, float y, float z){

    //distance to move in each direction
    float dist_x = 0;
    float dist_y = 0;
    float dist_z = 0;

    if(x != KEEP) dist_x = x - pos_now[limb][0]; //update distance to target in each dimension
    if(y != KEEP) dist_y = y - pos_now[limb][1];
    if(z != KEEP) dist_z = z - pos_now[limb][2];

    //calculate total distance from current point to new point
    float total_distance = sqrt(pow(dist_x,2) + pow(dist_y,2) + pow(dist_z,2));

    //divide distance in a direction by a scaled amount based on speed
    temp_speed[limb][0] = dist_x / total_distance * speed;
    temp_speed[limb][1] = dist_y / total_distance * speed;
    temp_speed[limb][2] = dist_z / total_distance * speed;

    //updated expected position if keeping position
    if(x != KEEP) pos_expected[limb][0] = x;
    if(y != KEEP) pos_expected[limb][1] = y;
    if(z != KEEP) pos_expected[limb][2] = z;

}

/*
 * Wait for Limb to reach the expected position.
 *
 * Also checks the timer_flag each time it loops until the limb
 * are where they should be.
 *
 * Input may ONLY be 0, 1, 2, or 3.
 */
void wait_reach(int limb){

    if(limb<0) assert(false);
    if(limb>3) assert(false);

    while(1){
        if(timer_flag)
            servo_service();
        if(pos_now[limb][0] == pos_expected[limb][0])
            if(pos_now[limb][1] == pos_expected[limb][1])
                if(pos_now[limb][2] == pos_expected[limb][2])
                    return;
    }
}

/*
 * wait for ALL limbs to reach expected position
 */
void wait_all_reach(void){
    int i;
    for(i=0; i<4; i++)
        wait_reach(i);
}



/*
 * Calculates the angle needed for the servos to position the foot at xyz
 *
 * t1 is Yaw of leg
 * t2 is pitch of leg
 *
 * Also handles some of the corrections necessary for desired coordinate system.
 * The Mathematical operations in this function are very intensive.
 *
 */
void xyz_to_angle(int limb, volatile float *t1, volatile float *t2, volatile float x, volatile float y, volatile float z){


    if(limb == LA){
        x = -x;
        y = -y;
    }

    if(limb == LL){
            x = -x;
            y = -y;
        }

    *t1 = (180/3.14) * atan2(y,x);
    *t2 = (180/3.14) * atan2(z,sqrt(pow(x,2) + pow(y,2))-A1);

}

/*
 * Corrects the angles generated by xyz_to_angle to be appropriate for the established coordinate system.
 *
 * This is the function that actually calls servo_write.
 */
void angle_to_servo(int limb, float t1, float t2){

    //transformation to be compatible w/ degree to count equation
    t1 += 90;
    t2 += 90;


    if(limb == RA) t2 = 180 - t2;
    if(limb == LL) t2 = 180 - t2;




    servo_write(Servo[limb][0], t1);
    servo_write(Servo[limb][1], t2);

}


/*
 * IRQ Handler for the TimerA interrupt-driven scheduler.
 */
void TA0_0_IRQHandler(void) {
    // Clear the compare interrupt flag
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    P1->OUT ^= BIT0;
    timer_flag = true;


}
