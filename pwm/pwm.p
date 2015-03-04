//pwm.p

//Copyright (C) 2013 ADLab - http://www.analogdigitallab.org/

// BBB port Assign Bit
// -------- ------ ------------
// P8.11    PWM0   PRU0_R30_15
// P8.12    PWM1   PRU0_R30_14

.origin 0
.entrypoint START

#include "pwm.hp"

#define PWM0 15
#define PWM1 14

START:
        LBCO  r0, CONST_PRUCFG, 4, 4            // Enable OCP master port
        CLR   r0, r0, 4
        SBCO  r0, CONST_PRUCFG, 4, 4

        MOV   r0, OWN_RAM                       // set CONST_PRUDRAM to point to DRAM0
        MOV   r1, PRU0_CTRL + CTBIR
        SBBO  r0, r1, 0, 4


        LBCO r0, CONST_PRUDRAM, 4, 4            // period

MAINLOOP:
        LBCO r1, CONST_PRUDRAM, 8, 4            // P8.11 duty cycle
        AND  r1, r1, 0x000000FF
        LBCO r2, CONST_PRUDRAM, 12, 4            // P8.12 duty cycle
        AND  r2, r2, 0x000000FF

        SET  r30, PWM0
        SET  r30, PWM1                   // turns on gpio

DELAY1:
        SUB  r1, r1, 1
        QBNE DELAY1, r1, 0

//        MOV r2, 0x00000100              // figure out how many cycles are left
//        RSB r1, r1, r2

        CLR  r30, PWM0
        CLR  r30, PWM1                  // turn off gpio

DELAY2:
        SUB  r2, r2, 1                  // loop remainder of period
        QBNE DELAY2, r0, 0


        SUB r0, r0, 1
        QBNE MAINLOOP, r0, 0

        // tell handler we're done
        MOV r31.b0, PRU0_ARM_INTERRUPT + 16
        // Halt the processor
        HALT
