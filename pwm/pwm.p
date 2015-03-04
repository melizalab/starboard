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

        LBCO r0, CONST_PRUDRAM, 0, 4
LOOP1:
        LBCO r1, CONST_PRUDRAM, 4, 4
        LBCO r2, CONST_PRUDRAM, 8, 4
        MIN  r2, r1, r2

        SET  r30, PWM1
DELAY1:
        SUB  r2, r2, 1
        QBNE DELAY1, r2, 0

        LBCO r2, CONST_PRUDRAM, 8, 4
        MIN  r2, r1, r2
        RSB  r2, r2, r1

        CLR  r30, PWM1

DELAY2:
        SUB  r2, r2, 1
        QBNE DELAY2, r2, 0

        LBCO r0, CONST_PRUDRAM, 0, 4
        QBNE LOOP1, r0, 0

        // tell handler we're done
        MOV r31.b0, PRU0_ARM_INTERRUPT + 16
        // Halt the processor
        HALT
