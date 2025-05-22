#pragma once
#include <Bluepad32.h>
#include "controls.hpp"
#include <Preferences.h>

void gp_onConnect(ControllerPtr ctl);
void gp_onDisconnect(ControllerPtr ctl);
void checkBootCountTimeout();

Preferences preferences;
ControllerPtr xbox = NULL;
void controls_setup() {
    BP32.setup(&gp_onConnect, &gp_onDisconnect);
    preferences.begin("bobot", false);
    int counter = preferences.getInt("bootCount", 0);
    if (counter > 0) {
        BP32.forgetBluetoothKeys();
        Serial.println("entering pairing mode");
    }
    preferences.putInt("bootCount", counter + 1);
}

bool connected() {
    return xbox != NULL && xbox->isConnected();
}

void read_controls() {
    vTaskDelay(10);
    yield();
    checkBootCountTimeout();
    BP32.update();
    if (!connected()) return;
    if (!xbox->hasData()) return;
    controls.throttle = xbox->throttle() - xbox->brake();
    controls.steering = abs(controls.throttle) * xbox->axisX();
    static int w = 0;
    static int p = 0;
    if (xbox->r1()) p = 1;
    if (xbox->l1()) p = 0;
    if (xbox->dpad() & DPAD_UP) w = 500;
    if (xbox->dpad() & DPAD_DOWN) w = -500;
    controls.weapon = 1500 + w * p;
}


void gp_onConnect(ControllerPtr ctl) {
    if (ctl->isGamepad()) {
        xbox = ctl;
        Serial.println("connected controller");
        BP32.enableNewBluetoothConnections(false);
    }
}

void gp_onDisconnect(ControllerPtr ctl) {
    if (ctl == xbox) {
        Serial.println("disconnected controller");
        xbox = NULL;
    }
}

void checkBootCountTimeout() {
    static bool timeouted = false;
    if (!timeouted && millis() > 5000) {
        preferences.putInt("bootCount", 0);
        timeouted = true;
    }
}
