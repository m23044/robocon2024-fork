#include <Arduino.h>
#include <MsTimer2.h>
#include <Servo.h>
#include <components/ims/IM920SL.h>
#include <components/motors/NonSpeedAdjustable.h>

NonSpeedAdjustable actions[] = {
    NonSpeedAdjustable(PIN_PD2, PIN_PD4), NonSpeedAdjustable(PIN_PD7, PIN_PB0),
    NonSpeedAdjustable(PIN_PC3, PIN_PC2), NonSpeedAdjustable(PIN_PC1, PIN_PC0),
    NonSpeedAdjustable(PIN_PC4, PIN_PC5)};

SerialPort serial(Serial);
IM920SL im(serial);

void emergencyStop() {
  for (auto &action : actions) {
    action.stop();
  }
}

void setup() {
  MsTimer2::set(IM_RECEIVE_INTERVAL_MILLIS, emergencyStop);
  MsTimer2::start();
  im.begin();
}

void loop() {}

void serialEvent() {
  MsTimer2::start();

  uint16_t keyCode;
  im.receive(keyCode);

  for (uint8_t i = 0; i < sizeof(actions) / sizeof(actions[0]); i++) {
    if (keyCode & (1 << i)) {
      if (i % 2 == 0)
        actions[i].forward();
      else
        actions[i].reverse();
    } else {
      actions[i].stop();
    }
  }
}