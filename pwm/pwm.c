/* * pwm.c * * Copyright (C) 2013 ADLab -
http://www.analogdigitallab.org/
Author: Haolin Li

* * * Redistribution and use in source
and binary forms, with or without * modification, are permitted provided
that the following conditions * are met: * * Redistributions of source
code must retain the above copyright * notice, this list of conditions
and the following disclaimer. * * Redistributions in binary form must
reproduce the above copyright * notice, this list of conditions and the
following disclaimer in the * documentation and/or other materials
provided with the * distribution. * * Neither the name of Texas
Instruments Incorporated nor the names of * its contributors may be used
to endorse or promote products derived * from this software without
specific prior written permission. * * THIS SOFTWARE IS PROVIDED BY THE
COPYRIGHT HOLDERS AND CONTRIBUTORS * "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT * LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR * A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT * OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT * LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, * DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY * THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT * (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE * OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. *
*/



/******************************************************************************
* Include Files *
******************************************************************************/
// Standard header files
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
//#include <math.h>
// Driver header file
#include "prussdrv.h"
#include <pruss_intc_mapping.h>
#include <math.h>

/******************************************************************************
* Local Macro Declarations *
******************************************************************************/
#define PRU_NUM          0

static void *pruDataMem;
static unsigned int *pruDataMem0; //AM33XX_DATA 8KB RAM1


int main(void){

        unsigned int ret;
        unsigned int value;
        unsigned int pwms;
        unsigned int delays;
        unsigned int start;

        tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
        printf("\nINFO: Starting %s example.\r\n", "PWM Demo with PRU");
        // Initialize the PRU
        prussdrv_init();
        // Open PRU Interrupt
        ret = prussdrv_open(PRU_EVTOUT_0);
        if (ret)
        {
                printf("prussdrv_open open failed\n");
                return (ret);
        }

        // Initialize interrupt
        prussdrv_pruintc_init(&pruss_intc_initdata);

        // assign the the data RAM address to pointers
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
        pruDataMem0 = (unsigned int*)pruDataMem;

        // set values
        pruDataMem0[0] = 1000;
        pruDataMem0[1] = 900;

        printf("\tINFO: Executing example.\r\n");
        prussdrv_exec_program(PRU_NUM, "./pwm.bin");

        // Wait until PRU0 has finished execution
        printf("\tINFO: Waiting for HALT command.\r\n");
        prussdrv_pru_wait_event(PRU_EVTOUT_0);
        printf("\tINFO: PRU completed transfer.\r\n");
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);

        printf("result: 0x%x\n", pruDataMem0[1]);

        // Disable PRU and close memory mapping
        prussdrv_pru_disable(PRU_NUM);
        prussdrv_exit();
        return(0);
}
