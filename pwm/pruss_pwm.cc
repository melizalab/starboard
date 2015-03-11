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

pwm::pwm(float period)
{
        tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

        prussdrv_init();
        _err = prussdrv_open(PRU_EVTOUT_0);
        // we can't throw errors in a node.js addon apparently, so just set a flag. dirty.
        if (_err)
                return;

        // Initialize interrupt
        prussdrv_pruintc_init(&pruss_intc_initdata);

        // assign the the data RAM address to pointers
        prussdrv_map_prumem(PRUSS0_PRU0_DATARAM, &_pruDataMem);
        _pruDataMem0 = reinterpret_cast<unsigned int*>(_pruDataMem);

        // set values
        _pruDataMem0[0] = 1;
        pwm::period(period);
        pwm::duty(0, 0);
        pwm::duty(1, 0);

}

pwm::~pwm()
{
        _pruDataMem0[0] = 0;
        usleep(period());
        prussdrv_pru_clear_event(PRU_EVTOUT_0, PRU0_ARM_INTERRUPT);
        prussdrv_pru_disable(PRU_NUM);
        prussdrv_exit();
}

bool
pwm::is_loaded() const
{
        return (_err == 0);
}

void
pwm::start(char const * path)
{
        prussdrv_exec_program(PRU_NUM, path);
}

float
pwm::period() const
{
        // PRU loop is 4 instructions except on cycles where the GPIO is flipped
        // on, so the actual period is going to be 4*_pruDataMem0[1] + 2 + 9. PRU
        // clock is 200 MHz, so instructions are 5 ns
        return (4.0 * _pruDataMem0[1] + 2 + 9) * PRU_STEP_USEC;
}

void
pwm::period(float usec)
{
        assert(usec > 0);
        // if period is greater than about 80 s the value will overflow
        assert(usec < 80e6);
        _pruDataMem0[1] = (unsigned int)round((usec / PRU_STEP_USEC - 2 - 9) / 4);
}

float
pwm::duty(unsigned int idx) const
{
        assert (idx < n_pwms);
        return (float)_pruDataMem0[2 + idx] / (float)_pruDataMem0[1] * 100;
}

void
pwm::duty(unsigned int idx, float duty)
{
        assert (idx < n_pwms);
        assert (duty >= 0 && duty <= 100);
        _pruDataMem0[2 + idx] = (unsigned int)round(duty / 100 * _pruDataMem0[1]);
}
