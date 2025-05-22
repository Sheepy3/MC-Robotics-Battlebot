#pragma once

#include <PPMReader.h>
#include <Arduino.h>
#include "controls.hpp"

#define N_CHANNELS 8
PPMReader ppm(29, N_CHANNELS);

#define CH_LSTICK_X 4
#define CH_LSTICK_Y 3
#define CH_RSTICK_X 1
#define CH_RSTICK_Y 2
#define CH_SW_A 5
#define CH_SW_B 6
#define CH_SW_C 7
#define CH_SW_D 8

void controls_setup() {
    // sets itself up automatically
}

bool connected() {
    return ppm.latestValidChannelValue(CH_LSTICK_X, -1) != -1;
}

void read_controls() {
    controls.weapon = ppm.latestValidChannelValue(CH_SW_C, -1);
    controls.steering = map(ppm.latestValidChannelValue(CH_LSTICK_X, -1), 1000, 2000, -512, 512);
    controls.throttle = map(ppm.latestValidChannelValue(CH_RSTICK_Y, -1), 1000, 2000, -512, 512);
}
