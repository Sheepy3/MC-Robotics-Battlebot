#include <PPMReader.h>
#include <SPI.h>
#include "L9958.hpp"

#define N_CHANNELS 8
PPMReader ppm(29, N_CHANNELS);

#define ESC_PWM 26
#define LEFT_PWM 27
#define RIGHT_PWM 28
#define LEFT_DIR 6
#define RIGHT_DIR 7

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
    Serial.println("serial init");
    SPI.begin();
    Serial.println("spi init");
    L9958::setup();
    Serial.println("L9958 init");
    // init motors
    pinMode(LEFT_PWM, OUTPUT);
    pinMode(RIGHT_PWM, OUTPUT);
    pinMode(LEFT_DIR, OUTPUT);
    pinMode(RIGHT_DIR, OUTPUT);
    analogWriteResolution(1024);
}

void loop() {
    // for (int channel = 1; channel <= N_CHANNELS; channel++) {
    //     int value = ppm.latestValidChannelValue(channel, -1);
    //     Serial.printf("%i\t", value);
    // }
    // Serial.println();
    // L9958::update();
    // Serial.printf("%hi %hi\n", L9958::dataIn[0], L9958::dataIn[1]);
    int speedPPM = ppm.latestValidChannelValue(CH_RSTICK_Y, -1);
    int steeringPPM = ppm.latestValidChannelValue(CH_RSTICK_X, -1);
    if (speedPPM == -1 || steeringPPM == -1) {
        analogWrite(LEFT_PWM, 0);
        analogWrite(RIGHT_PWM, 0);
        return;
    }
    int speed = map(speedPPM, 1000, 2000, -512, 512);
    int steering = map(steeringPPM, 1000, 2000, -512, 512);
    int leftPower = speed + steering;
    int rightPower = speed - steering;
    digitalWrite(LEFT_DIR, leftPower < 0);
    analogWrite(LEFT_PWM, abs(leftPower));
    digitalWrite(RIGHT_DIR, rightPower > 0);
    analogWrite(RIGHT_PWM, abs(rightPower));
}
