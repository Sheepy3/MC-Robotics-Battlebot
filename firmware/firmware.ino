#include <PPMReader.h>

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

void setup() {
    Serial.begin(115200);
}

void loop() {
    for (int channel = 1; channel <= N_CHANNELS; channel++) {
        unsigned int value = ppm.latestValidChannelValue(channel, 0);
        Serial.printf("%u\t", value);
    }
    Serial.println();
}
