//pwm.p

//Copyright (C) 2013 ADLab - http://www.analogdigitallab.org/

// BBB Schematic  BBB port Assign Bit
// -------------  -------- ------ ------------
// LCD_DATA0      P8.45    PWM0     PRU1_R30_0
// LCD_DATA1      P8.46    PWM1     PRU1_R30_1
// LCD_DATA2      P8.43    PWM2     PRU1_R30_2
// LCD_DATA3      P8.44    PWM3     PRU1_R30_3



.origin 0
.entrypoint START

#include "pwm.hp"

#define GPIO1 0x4804c000
#define GPIO_CLEARDATAOUT 0x190
#define GPIO_SETDATAOUT 0x194
#define MEM_START 0x00000100

START:

// Enable OCP master port
LBCO      r0, CONST_PRUCFG, 4, 4
CLR     r0, r0, 4         // Clear SYSCFG[STANDBY_INIT] to enable OCP master port
SBCO      r0, CONST_PRUCFG, 4, 4

// Configure the programmable pointer register for PRU1 by setting c28_pointer[15:0]
// field to 0x0100.  This will make C28 point to 0x00010000 (PRU shared RAM).
MOV     r0, 0x00000100
MOV       r1, CTPPR_0
ST32      r0, r1

MOV r0, 0x00100000
MOV r1, CTPPR_1
ST32 r0, r1



// Change the following code accordingly
//**************


        SBCO 0x00000000,CONST_PRUDRAM,12, 4

MAINLOOP:
        LBCO r4,CONST_PRUDRAM,12,4
        QBNE MAINLOOP,r4,1    //waiting for start


        LBCO r0,CONST_PRUDRAM,4,4    //delay
        LBCO r3,CONST_PRUDRAM,0,4    //PWM mask


LOOP1:
        SET r30,r3                      // turns on gpio

        LBCO r1,CONST_PRUDRAM,8,4       // read duty cycle
        AND r1, r1, 0x000000FF
DELAY2:
        SUB r1, r1 , 1                  // loop duty cycle times
        QBNE DELAY2, r1, 0

        CLR r30,r3                      // turn off gpio

        LBCO r1,CONST_PRUDRAM,8,4       // read duty cycle
        AND r1, r1, 0x000000FF

        MOV r2, 0x00000100              // figure out how many cycles are left
        RSB r1, r1, r2
DELAY3:
        SUB r1, r1 , 1                  // loop remainder of period
        QBNE DELAY3, r1, 0

        SUB r0, r0, 1                   // main loop
        QBNE LOOP1, r0, 0

//***********************

        MOV r31.b0, PRU1_ARM_INTERRUPT+16  //tell the c program we are done
//(just remove it if your c program does not handle the interrupt)
        HALT      // Halt the processor
