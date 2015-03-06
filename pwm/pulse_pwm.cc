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

#include <iostream>

#include "pruss_pwm.hh"

using pruss::pwm;
using namespace std;

int main(int argc, char ** argv)
{
        if (argc < 5) {
                cout << "Usage: " << argv[0] << " duty_1 duration_1 duty_2 duration_2" << endl;
        }

        pwm PWM(50, 0, 0);
}
