
//ATTENTION!!!!
//PLEASE IGNORE BELOW DIAGRAM!!!
//THis diagram was back when I was using my own Naming Convention
//Check servo.h for servo channels



/*This Program is designed to allow all the servos to be attached
 * to the skeleton in a predictable way.
 *
 * In order to have proper orientation and operation of the robot,
 * a consistent convention must be established. Below is a top-down
 * view of the robot. The boxes represent each servo and the name
 * assigned to it. Each Group (A-D) represents a limb, while each
 * number represents a joint (1 is shoulder, 2 is knee)
 *
 *    ____                         ____
 *   /    \                       /    \
 *  /  A2  \      TOP-DOWN       /  B2  \
 *  \______/                     \______/
 *         \ ______       ______ /
 *          |      |     |      |
 *          |  A1  |     |  B1  |
 *          |______|     |______|
 *              |           |
 *  FRONT ------------------------- BACK
 *           ___|__       __|___
 *          |      |     |      |
 *          |  D1  |     |  C1  |
 *          |______|     |______|
 *     ____/                     \____
 *    /    \                     /    \
 *   /  D2  \                   /  C2  \
 *   \______/                   \______/
 *
 * It is very important that all the Shoulder-Knee joints get assembled
 * EXACTLY the same. This will guarantee that all the Knee servos behave
 * the same.
 *
 * When the program is initially started, the servos will move to their
 * center postiton (90 degrees).
 *
 * For the first time using this program
 * MAKE SURE THAT NO SERVOS ARE SECURED TO THE SKELETON!! There is a
 * serious risk that the servos will collide with each other if they
 * aren't secured in the correct position.
 *
 * Once all the servos are centered, it is time to attach the servos
 * the skeleton in what is called the Neutral Position. In the Neutral
 * Position, the horn of the servo should be be in-line or parallel to
 * the body of the servo (or as close as possible). The Horn of the
 * servo is used to secure the servo in the correct position
 *
 * Below is the pinning information to connect servos to PCA9685 board
 *
 *   A1-->Group A, servo 1 to servo pin 0 (of PCA9685)
 *   A2-->Group A, servo 2 to servo pin 1 (of PCA9685)
 *   B1-->Group B, servo 1 to servo pin 2 (of PCA9685)
 *   B2-->Group B, servo 2 to servo pin 3 (of PCA9685)
 *   C1-->Group C, servo 1 to servo pin 4 (of PCA9685)
 *   C2-->Group C, servo 2 to servo pin 5 (of PCA9685)
 *   D1-->Group D, servo 1 to servo pin 6 (of PCA9685)
 *   D2-->Group D, servo 2 to servo pin 7 (of PCA9685)
 *
 * Once all the servos are secured in the Neutral Position,
 * pressing the button (S1) on the MSP432 will cause the robot to stand
 * up.
 *
 * IF THE ROBOT DOES NOT STAND UP CORRECTLY, SOMETHING ELSE IS WRONG!
 * Check:
 *       -Servo Orientation at the knee joints
 *       -Wiring to PCA
 *       -Servo Neutral Position
 *
 * MSP Wiring:
 *
 *         msp432p401r
 *      -----------------
 *     |                 |
 *     |                 |
 *     |             P1.7|-->SCL (PCA9685)
 *     |             P1.6|-->SDA (PCA9685)
 *     |                 |
 *     |                 |
 *     |              3V3|-->VCC (PCA9685)
 *     |              GND|-->GND (PCA9685)
 *     |                 |
 *     |_________________|
 *
 *
 *
 *  By: Tyler Davidson
 *  Date: July 17th, 2020
 *
 *  Modified by: Terry Vis
 *  Date: Nov 9, 2020
 *
 */

#include "msp.h"
#include "i2c.h"
#include "pca9685.h"
#include "servo.h"
#include "stIMU.h"
#include "stdio.h"
#include "assert.h"

#include "uart.h"
#include "RFlink.h"



volatile int state = 0;
/*
 * States:
 * 0 - Quade neutral (don't write data to servos)
 * 1 - Read data from Rx UART buffer, make calculations
 * 2 - Write data to servos
 */

static RX_UART_BUF_TypeDef rx_uart_buf;
static UART_PAYLOAD_TypeDef uart_payload_struct;
static RX_UART_TypeDef rx_uart_struct;

void main(void)
 {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//set up Button Interrupt
    P1->DIR &= ~BIT1;
    P1->OUT |= BIT1;
    P1->REN |= BIT1;
    P1->IES |= BIT1;
    P1->IFG = 0;
    P1->IE  |= BIT1;

    // Configure data for Rx buffer
    rx_uart_buf.size = 8;
    rx_uart_buf.read = 0;
    rx_uart_buf.write = 0;

    //enable interrupts
    NVIC_EnableIRQ(PORT1_IRQn);
	__enable_irq();

    I2C_OPEN_STRUCT_TypeDef i2c_open_struct;

    i2c_open_struct.uca10 = 0;                              //no self 10-bit Address
    i2c_open_struct.ucsla10 = 0;                            //no 10-bit Addressing
    i2c_open_struct.ucmm = 0;                               //No Multi-Master
    i2c_open_struct.ucmst = EUSCI_B_CTLW0_MST;              //Master Mode
    i2c_open_struct.ucsselx = EUSCI_B_CTLW0_SSEL__SMCLK;    //SMCLK to be selected (3MHz)
    i2c_open_struct.ucbrx = 30;                             //Prescaler for Selected Clock.
                                                            //(100kHz)

    //This sets up the I2C driver to operate with the
    //correct settings.
    //EUSCI_B0 uses P1.7 as SCL and P1.6 as SDA
    i2c_open(EUSCI_B0, &i2c_open_struct);

    //pca9685_init(); //sets up PCA to output at the correct frequency
    config_LIS3MDL();

    int16_t mx, my, mz;
    uint8_t data;
    int i;


    //Neutral Position
    while(state == 0){
        for(i=0; i<200000; i++);
        //i2c_start(EUSCI_B0, LIS3MDL_MAG_I2C_ADDRESS, READ, &data, 1, LIS3MDL_MAG_STATUS_REG);
        i2c_start(EUSCI_B0, LIS3MDL_MAG_I2C_ADDRESS, READ, &data, 1, LIS3MDL_MAG_STATUS_REG);
        printf("status 1: %d\n",data);
        for(i=0; i<1000; i++);
        mx = read_magnetometer_x();
        my = read_magnetometer_y();
        mz = read_magnetometer_z();
        printf("x: %d\n", mx);
        printf("y: %d\n", my);
        printf("z: %d\n", mz);
        //printf("status: %d\n", data);
        printf("\n");
        printf("\n");
        //printf("\n");
        //printf("\n");
    }

    //Standing Position -- any state except 0
    while(state){
        /*servo_write(URL,90-45); //Because the Orientations are opposite of the other servos,
        servo_write(LRL,90-45); //the direction the servo must move to stand up is also opposote
        servo_write(URA,90+45);
        servo_write(LRA,90+45);
        servo_write(ULL,90+45);
        servo_write(LLL,90+45);
        servo_write(ULA,90-45); //See Above Comment
        servo_write(LLA,90-45);*/
    }

}

// putting this here for ease of struct/buffer handling
void EUSCIA0_IRQHandler(void) {
    // check each flag, then perform necessary function
    if (UART_P->IFG & EUSCI_A_IFG_RXIFG) {

        UART_P->CTLW0 |= EUSCI_A_CTLW0_TXBRK;

        // store RXBUF bits, will empty buffer
        uint8_t rx = UART_P->RXBUF & EUSCI_A_RXBUF_RXBUF_MASK;
        *uart_payload_struct.data = rx;

        if (rx_uart_struct.i == rx_uart_struct.length) {
            int err = rx_buf_write(&rx_uart_buf, rx_uart_struct);
            assert (!err);
            rx_uart_struct.i = 0; // reset iteration variable
        }

        rx_state(rx, &rx_uart_struct);

        UART_P->IFG &= ~EUSCI_A_IFG_RXIFG; // clear flag

    } else if (!(UART_P->RXBUF) && !(UART_P->IFG & EUSCI_A_IFG_RXIFG)) {
        // if RXBUF is empty and RX interrupt flag is cleared, start transmission of
        // next word
        UART_P->CTLW0 &= ~EUSCI_A_CTLW0_TXBRK;
    }
}

/* Port1 ISR */
void PORT1_IRQHandler(void){
    volatile uint32_t j;

    //Change State to Standing Position
    if(P1->IFG & BIT1)
        state = 1;

    // Delay for switch debounce
    for(j = 0; j < 100000; j++);

    P1->IFG &= ~BIT1;
}
 /*servo_write(URL,90); //90 Degrees represents the Neutral Position
        servo_write(LRL,90);
        servo_write(URA,90);
        servo_write(LRA,90);
        servo_write(ULL,90);
        servo_write(LLL,90);
        servo_write(ULA,90);
        servo_write(LLA,90);

        */
