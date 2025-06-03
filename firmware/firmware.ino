#include <SPI.h>
#include "L9958.hpp"

#ifdef ARDUINO_ARCH_RP2040
#include "controls_radio.hpp"
#include <Servo.h>
#define ESC_PWM 26
#define LEFT_PWM 27
#define RIGHT_PWM 28
#define LEFT_DIR 6
#define RIGHT_DIR 7
#elif ARDUINO_ARCH_ESP32
#include "controls_bluepad32.hpp"
#include <ESP32Servo.h>
#define ESC_PWM 0
#define LEFT_PWM 1
#define RIGHT_PWM 2
#define LEFT_DIR 4
#define RIGHT_DIR 5
#else
#error "wrong platform selected"
#endif

Servo esc;

void setup() {
    Serial.begin(115200);
    Serial.println("serial init");
    SPI.begin();
    Serial.println("spi init");
    L9958::setup();
    pinMode(LEFT_PWM, OUTPUT);
    pinMode(RIGHT_PWM, OUTPUT);
    pinMode(LEFT_DIR, OUTPUT);
    pinMode(RIGHT_DIR, OUTPUT);
    analogWriteResolution(9);
    Serial.println("L9958 init");
    controls_setup();
    Serial.println("controls init");
    esc.attach(ESC_PWM);
    esc.writeMicroseconds(1500);
    Serial.println("esc init");
}

bool initted = false;
void loop() {
    read_controls();
    L9958::update();
    Serial.printf("%#hX %#hX\n", L9958::dataIn[0], L9958::dataIn[1]);
    if (!connected()) {
        analogWrite(LEFT_PWM, 0);
        analogWrite(RIGHT_PWM, 0);
        esc.writeMicroseconds(1500);
        initted = false;
        return;
    }
    if (!initted && abs(controls.weapon - 1500) > 10) {
        // angry beep
        digitalWrite(LEFT_PWM, 1);
        tone(LEFT_DIR, 1000);
        digitalWrite(RIGHT_PWM, 1);
        tone(RIGHT_DIR, 1000);
        delay(250);
        analogWrite(LEFT_PWM, 0);
        analogWrite(RIGHT_PWM, 0);
        noTone(LEFT_DIR);
        noTone(RIGHT_DIR);
        delay(250);
        return;
    }
    initted = true;
    int speed = controls.throttle;
    int steering = controls.steering;
    int leftPower = speed + steering;
    int rightPower = speed - steering;
    digitalWrite(LEFT_DIR, leftPower < 0);
    analogWrite(LEFT_PWM, abs(leftPower));
    digitalWrite(RIGHT_DIR, rightPower > 0);
    analogWrite(RIGHT_PWM, abs(rightPower));
    esc.writeMicroseconds(controls.weapon);
}
