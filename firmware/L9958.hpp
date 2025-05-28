namespace L9958 {

typedef enum {
    L9958_CURRENT_LIMIT_2A = 0,
    L9958_CURRENT_LIMIT_4A = 1,
    L9958_CURRENT_LIMIT_6A = 2,
    L9958_CURRENT_LIMIT_8A = 3,
} current_limit_t;

typedef struct {
    bool diagReset;
    current_limit_t currentLimit;
    bool limitVoltageSlewRate;
    bool limitCurrentSlewRate;
    bool currentSlewRateDisable;
    bool doOpenLoadTest;
} control_reg;

typedef struct {
    bool openLoadWhenOff;
    bool openLoadWhenOn;
    bool undervolted;
    bool overvolted;
    bool overCurrent;
    bool tooHot;
    bool overheated;
    bool isOn;
    bool overcurrentLowSide1;
    bool overcurrentLowSide2;
    bool overcurrentHighSide1;
    bool overcurrentHighSide2;
    bool shortedToGround;
    bool shortedToVcc;
} status_reg;

#include <Arduino.h>
#include <SPI.h>

// hard-coded to two L9958s.

status_reg status[2];
control_reg ctl[2];
uint16_t dataOut[2] = { 0, 0 };
uint16_t dataIn[2] = { 0, 0 };

#define L9958_CS 1

void setup() {
    pinMode(L9958_CS, OUTPUT);
    digitalWrite(L9958_CS, HIGH);
}

void update() {
    SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE1));
    digitalWrite(L9958_CS, LOW);
    memset(dataOut, 0, sizeof(dataOut));
    for (int i = 0; i < 2; i++)
        dataOut[i] = (ctl[i].diagReset << 1) | (ctl[i].currentLimit << 2) | (ctl[i].limitVoltageSlewRate << 8) | (ctl[i].limitCurrentSlewRate << 9) | (ctl[i].currentSlewRateDisable << 10) | (ctl[i].doOpenLoadTest << 11);
    dataIn[0] = SPI.transfer16(dataOut[0]);
    dataIn[1] = SPI.transfer16(dataOut[1]);
    for (int i = 0; i < 2; i++) {
        status[i].openLoadWhenOff = (dataIn[i] >> 0) & 1;
        status[i].openLoadWhenOn = (dataIn[i] >> 1) & 1;
        status[i].undervolted = (dataIn[i] >> 2) & 1;
        status[i].overvolted = (dataIn[i] >> 3) & 1;
        status[i].overCurrent = (dataIn[i] >> 4) & 1;
        status[i].tooHot = (dataIn[i] >> 5) & 1;
        status[i].overheated = (dataIn[i] >> 6) & 1;
        status[i].isOn = (dataIn[i] >> 7) & 1;
        status[i].overcurrentLowSide1 = (dataIn[i] >> 8) & 1;
        status[i].overcurrentLowSide2 = (dataIn[i] >> 9) & 1;
        status[i].overcurrentHighSide1 = (dataIn[i] >> 10) & 1;
        status[i].overcurrentHighSide1 = (dataIn[i] >> 11) & 1;
        status[i].shortedToGround = (dataIn[i] >> 14) & 1;
        status[i].shortedToVcc = (dataIn[i] >> 15) & 1;
    }
    SPI.endTransaction();
    digitalWrite(L9958_CS, HIGH);
}

}
