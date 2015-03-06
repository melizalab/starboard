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

#include <cstdio>
#include <iostream>

#include "pruss_pwm.hh"

using pruss::pwm;
using namespace std;

int main(int argc, char ** argv)
{
        unsigned int idx;
        float duty_0, dur_0, duty_1, dur_1;

        if (argc < 6) {
                cout << "Usage: " << argv[0] << " pwm_idx duty_1 duration_1 duty_2 duration_2" << endl;
                return (-1);
        }
        sscanf(argv[1], "%u", &idx);
        sscanf(argv[2], "%f", &duty_0);
        sscanf(argv[3], "%f", &dur_0);
        sscanf(argv[4], "%f", &duty_1);
        sscanf(argv[5], "%f", &dur_1);

        pwm PWM(50);

        cout << "pwm period: " << PWM.period() << " μs" << endl;
        PWM.duty(idx, duty_0);
        usleep(dur_0 * 1000);
        PWM.duty(idx, duty_1);
        usleep(dur_1 * 1000);
}
