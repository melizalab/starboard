/*
 * PRU PWM control
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Copyright (C) 2015 C Daniel Meliza <dan || meliza.org>
 */

#include <stdexcept>
#include <cmath>
#include <cassert>

// PRUSS drivers
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#include "pruss_pwm.hh"

#define PRU_NUM 0
#define PRU_FIRMWARE "./pwm.bin"
#define PRU_STEP_USEC 0.005

using namespace pruss;

pwm::pwm(float period, float pwm0_duty, float pwm1_duty)
{
        int ret;
        tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

        prussdrv_init();
        ret = prussdrv_open(PRU_EVTOUT_0);
        if (ret)
                throw std::runtime_error("prussdrv_open failed");

        // Initialize interrupt
        prussdrv_pruintc_init(&pruss_intc_initdata);

        // assign the the data RAM address to pointers
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &pruDataMem);
        pruDataMem0 = (unsigned int*)pruDataMem;

        // set values
        pruDataMem0[0] = 1;
        pwm::period(period);
        pwm::pwm0_duty(pwm0_duty);
        pwm::pwm1_duty(pwm1_duty);

        prussdrv_exec_program(PRU_NUM, PRU_FIRMWARE);

}

pwm::~pwm()
{
        pruDataMem0[0] = 0;
        usleep(period());
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
        prussdrv_pru_disable(PRU_NUM);
        prussdrv_exit();
}

float pwm::period() const
{
        // PRU loop is 4 instructions except on cycles where the GPIO is flipped
        // on, so the actual period is going to be 4*pruDataMem0[1] + 2 + 9. PRU
        // clock is 250 MHz, so instructions are 5 ns
        return (4.0 * pruDataMem0[1] + 2 + 9) * PRU_STEP_USEC;
}

void pwm::period(float usec)
{
        assert(usec > 0);
        // if period is greater than about 80 s the value will overflow
        assert(usec < 80e6);
        pruDataMem0[1] = (unsigned int)round((usec / PRU_STEP_USEC - 2 - 9) / 4);
}

float pwm::pwm0_duty() const
{
        return (float)pruDataMem0[2] / (float)pruDataMem0[1];
}

void pwm::pwm0_duty(float duty)
{
        assert(duty >= 0 && duty <= 100);
        pruDataMem0[2] = (unsigned int)round(duty * period());
}

float pwm::pwm1_duty() const
{
        return (float)pruDataMem0[3] / (float)pruDataMem0[1];
}

void pwm::pwm1_duty(float duty)
{
        pruDataMem0[3] = (unsigned int)round(duty * period());
}
