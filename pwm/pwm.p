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
        LBCO r0, CONST_PRUCFG, 4, 4            // Enable OCP master port
        CLR  r0, r0, 4
        SBCO r0, CONST_PRUCFG, 4, 4

        MOV  r0, OWN_RAM                       // set CONST_PRUDRAM to point to DRAM0
        MOV  r1, PRU0_CTRL + CTBIR
        SBBO r0, r1, 0, 4

        LBCO r0, CONST_PRUDRAM, 0, 4           // enable
LOOP1:
        LBCO r1, CONST_PRUDRAM, 4, 4           // period
        LBCO r2, CONST_PRUDRAM, 8, 4           // duty cycle PWM 1
        LBCO r3, CONST_PRUDRAM, 12, 4          // duty cycle PWM 2
        MIN  r2, r1, r2
        MIN  r3, r1, r3

        CLR  r30, PWM0
        CLR  r30, PWM1

DELAY1:
        QBNE NEXT1, r1, r2
        // turn on GPIO when counter == duty cycle
        SET  r30, PWM0
NEXT1:
        QBNE NEXT2, r1, r3
        SET  r30, PWM1
NEXT2:
        SUB  r1, r1, 1
        QBNE DELAY1, r1, 0

        // check enable flag
        LBCO r0, CONST_PRUDRAM, 0, 4
        QBNE LOOP1, r0, 0

        // turn off GPIOs
        CLR  r30, PWM0
        CLR  r30, PWM1
        // tell handler we're done
        MOV r31.b0, PRU0_ARM_INTERRUPT + 16
        // Halt the processor
        HALT
