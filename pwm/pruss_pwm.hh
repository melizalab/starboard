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

#ifndef _PRU_PWM_HH
#define _PRU_PWM_HH

namespace pruss {

class pwm {

public:
        /**
         * Load PWM firmware in PRU and start it
         *
         * period - period of cycle, in usec
         * pwm0_duty - duty cycle for pwm0
         * pwm1_duty - duty cycle for pwm1
         */
        pwm(float period);
        ~pwm();

        void start(char const * path);
        bool running() const;

        float period() const;
        void period(float);

        float duty(unsigned int pwm) const;
        void duty(unsigned int pwm, float duty);

        const static unsigned int n_pwms = 2;

private:
        int _err;
        void * _pruDataMem;
        unsigned int * _pruDataMem0;

};

}

#endif
