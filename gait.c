/*
 * gait.c
 *
 *  Created on: Nov 18, 2020
 *      Author: terry
 */

#include "gait.h"

// Moves Quade in a certain direction given x and y dimensions of joystick
void servo_gait(int8_t * pos) {
    //---------------------------------------------------------------------------
    // Vertical first
    //---------------------------------------------------------------------------

    // Up
    if (pos[0] == JS_UP) {
        //1
        set_pos(RA, 50, 0, -60);
        set_pos(RL, 50, 0, -70);
        wait_all_reach();

        //2
        set_pos(RA, 50, 100, 0);
        wait_reach(RA);

        //3
        set_pos(RL, KEEP, -50, KEEP);
        set_pos(LA, KEEP, 0, KEEP);
        set_pos(LL, 0, -50, KEEP);
        set_pos(RA, KEEP, 50, -60);
        wait_all_reach();

        //4
        set_pos(LL, KEEP, KEEP, 0);
        wait_reach(LL);
        set_pos(LL, -50, 0, KEEP);
        wait_reach(LL);
        set_pos(LL, KEEP, KEEP, -60);
        wait_reach(LL);

        //5
        set_pos(LA, -50, 100, 0);
        wait_reach(LA);

        //6
        set_pos(RA, KEEP, 0, KEEP);
        set_pos(RL, KEEP, -100, KEEP);
        set_pos(LL, KEEP, -50, KEEP);
        set_pos(LA, -50, 50, -70);
        wait_all_reach();

        //7
        set_pos(RL, 50, 0, -70);
        wait_reach(RL);

        //8
        set_pos(RA, 50, 50, -60);
        set_pos(LL, -50, -50, -61);
        set_pos(LA, -50, 50, -71);
        wait_all_reach();
    }

    // Down
    else if (pos[0] == JS_DOWN) {
        //1
        set_pos(RA, 50, 0, -60);
        set_pos(RL, 50, 0, -70);
        wait_all_reach();

        //2
        set_pos(RL, 50, -100, 0);
        wait_reach(RL);

        //3
        set_pos(RA, KEEP, 50, KEEP);
        set_pos(LL, KEEP, 0, KEEP);
        set_pos(LA, 0, 50, KEEP);
        set_pos(RL, KEEP, -50, -60);
        wait_all_reach();

        //4
        set_pos(LA, KEEP, KEEP, 0);
        wait_reach(LA);
        set_pos(LA, -50, 0, KEEP);
        wait_reach(LA);
        set_pos(LA, KEEP, KEEP, -60);
        wait_reach(LA);

        //5
        set_pos(LL, -50, -100, 0);
        wait_reach(LL);

        //6
        set_pos(RL, KEEP, 0, KEEP);
        set_pos(RA, KEEP, 100, KEEP);
        set_pos(LA, KEEP, 50, KEEP);
        set_pos(LL, -50, -50, -70);
        wait_all_reach();

        //7
        set_pos(RA, 50, 0, -70);
        wait_reach(RA);

        //8
        set_pos(RL, 50, -50, -60);
        set_pos(LA, -50, 50, -61);
        set_pos(LL, -50, -50, -71);
        wait_all_reach();
    }

    // Standing position--want to avoid any errors in next movement due to instability
    servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
    servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
    servo_write(URA,90+45);
    servo_write(LRA,90+45);
    servo_write(ULL,90+45);
    servo_write(LLL,90+45);
    servo_write(ULA,90-45); //See Above Comment
    servo_write(LLA,90-45);
    wait_all_reach();

    //------------------------------------------------------------------------
    // Horizontal next
    //------------------------------------------------------------------------

    // Left
    if (pos[1] == JS_LEFT) {
        //1
        set_pos(LA, 0, 50, -60);
        set_pos(RA, 0, 50, -70);
        wait_all_reach();

        //2
        set_pos(LA, -100, 50, 0);
        wait_reach(LA);

        //3
        set_pos(RA, 50, KEEP, KEEP);
        set_pos(LL, 0, KEEP, KEEP);
        set_pos(RL, 50, 0, KEEP);
        set_pos(LA, -50, KEEP, -60);
        wait_all_reach();

        //4
        set_pos(RL, KEEP, KEEP, 0);
        wait_reach(RL);
        set_pos(RL, 0, -50, KEEP);
        wait_reach(RL);
        set_pos(RL, KEEP, KEEP, -60);
        wait_reach(RL);

        //5
        set_pos(LL, -100, -50, 0);
        wait_reach(LL);

        //6
        set_pos(LA, 0, KEEP, KEEP);
        set_pos(RA, 100, KEEP, KEEP);
        set_pos(RL, 50, KEEP, KEEP);
        set_pos(LL, -50, -50, -70);
        wait_all_reach();

        //7
        set_pos(RA, 0, 50, -70);
        wait_reach(RA);

        //8
        set_pos(LA, -50, 50, -60);
        set_pos(RL, 50, -50, -61);
        set_pos(LL, -50, -50, -71);
        wait_all_reach();
    }

    // Right
    else if (pos[1] == JS_RIGHT) {
        //1
        set_pos(RA, 0, 50, -60);
        set_pos(LA, 0, 50, -70);
        wait_all_reach();

        //2
        set_pos(RA, 100, 50, 0);
        wait_reach(RA);

        //3
        set_pos(LA, -50, KEEP, KEEP);
        set_pos(RL, 0, KEEP, KEEP);
        set_pos(LL, -50, 0, KEEP);
        set_pos(RA, 50, KEEP, -60);
        wait_all_reach();

        //4
        set_pos(LL, KEEP, KEEP, 0);
        wait_reach(LL);
        set_pos(LL, 0, -50, KEEP);
        wait_reach(LL);
        set_pos(LL, KEEP, KEEP, -60);
        wait_reach(LL);

        //5
        set_pos(RL, 100, -50, 0);
        wait_reach(RL);

        //6
        set_pos(RA, 0, KEEP, KEEP);
        set_pos(LA, -100, KEEP, KEEP);
        set_pos(LL, -50, KEEP, KEEP);
        set_pos(RL, 50, -50, -70);
        wait_all_reach();

        //7
        set_pos(LA, 0, 50, -70);
        wait_reach(LA);

        //8
        set_pos(RA, 50, 50, -60);
        set_pos(LL, -50, -50, -61);
        set_pos(RL, 50, -50, -71);
        wait_all_reach();
    }

    // Standing position--see above comment (line 112)
    servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
    servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
    servo_write(URA,90+45);
    servo_write(LRA,90+45);
    servo_write(ULL,90+45);
    servo_write(LLL,90+45);
    servo_write(ULA,90-45); //See Above Comment
    servo_write(LLA,90-45);
    wait_all_reach();
}


