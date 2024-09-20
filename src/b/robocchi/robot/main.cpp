#include "../controller/Controller.h"
#include <Arduino.h>
#include <TimerOne.h>
#include <components/ims/ImReceiver.h>
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>
#include <components/servos/MG90S.h>

#define ONE_LEVEL_DEGREE 45

Controller controller;

// 使用可能なピン:
// PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC2、PIN_PC3、PIN_PB1、PIN_PB2
MG90S armL;
MG90S armR;
TB67H450 motorL(PIN_PD2, PIN_PD4);
TB67H450 motorR(PIN_PC2, PIN_PC3);
TB67H450 jumpper(PIN_PD7, PIN_PB0);

void emergencyStop() {
  motorL.stop();
  motorR.stop();
  jumpper.stop();
  Timer1.stop();
}

void setup() {
  // 0.5秒ごとに割り込みを発生させる
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS);
  Timer1.attachInterrupt(emergencyStop); // 割り込みハンドラを設定
  Timer1.start();                        // 割り込みを開始
  armL.attach(PIN_PB1);
  armR.attach(PIN_PB2);
}

void loop() {
  armL.write(ONE_LEVEL_DEGREE * controller.armLevelL);
  armR.write(ONE_LEVEL_DEGREE * controller.armLevelR);
}

void serialEvent() {
  Timer1.restart(); // 割り込みを再開
  static ImReceiver imReceiver(Serial);
  imReceiver.receive(controller);

  if (controller.forwardL)
    motorL.forward();
  else if (controller.reverseL)
    motorL.reverse();
  else
    motorL.stop();

  if (controller.forwardR)
    motorR.forward();
  else if (controller.reverseR)
    motorR.reverse();
  else
    motorR.stop();

  if (controller.jump)
    jumpper.forward();
  else if (controller.jumpReady)
    jumpper.reverse();
  else
    jumpper.stop();
}