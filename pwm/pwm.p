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


MAINLOOP:
        LBCO r0, CONST_PRUDRAM, 0, 4            // period
        LBCO r4, CONST_PRUDRAM, 4, 4            // P8.11 duty cycle
        LBCO r5, CONST_PRUDRAM, 8, 4            // P8.12 duty cycle

LOOP1:
        SET r30, PWM0
        SET r30, PWM1                   // turns on gpio

/*         LBCO r1,CONST_PRUDRAM,8,4       // read duty cycle */
/*         AND r1, r1, 0x000000FF */
/* DELAY2: */
/*         SUB r1, r1 , 1                  // loop duty cycle times */
/*         QBNE DELAY2, r1, 0 */


/*         LBCO r1,CONST_PRUDRAM,8,4       // read duty cycle */
/*         AND r1, r1, 0x000000FF */

/*         MOV r2, 0x00000100              // figure out how many cycles are left */
/*         RSB r1, r1, r2 */
DELAY3:
        SUB  r0, r0 , 1                  // loop remainder of period
        QBNE DELAY3, r0, 0

        CLR  r30, PWM0
        CLR  r30, PWM1                  // turn off gpio


/*         SUB r0, r0, 1                   // main loop */
/*         QBNE LOOP1, r0, 0 */

        // tell handler we're done
        MOV r31.b0, PRU0_ARM_INTERRUPT + 16
        // Halt the processor
        HALT
