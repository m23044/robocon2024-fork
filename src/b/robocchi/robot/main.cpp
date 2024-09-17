#include "../controller/Controller.h"
#include <Arduino.h>
#include <Servo.h>
#include <TimerOne.h>
#include <components/ims/ImReceiver.h>
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>

Controller controller;

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PC1, PC0
Servo armL;
Servo armR;
TB67H450 motorL(PD2, PD4);
TB67H450 motorR(PD7, PB0);
BD62193 jumpper(PC3, PC2);

void emergencyStop() {
  motorL.stop();
  motorR.stop();
  armL.write(45 * controller.armLevel);
  armR.write(-45 * controller.armLevel);
  jumpper.stop();
  Timer1.stop();
}

void setup() {
  Timer1.initialize(500000); // 0.5秒ごとに割り込みを発生させる
  Timer1.attachInterrupt(emergencyStop); // 割り込みハンドラを設定
  Timer1.start();                        // 割り込みを開始
}

void loop() {}

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

  armL.write(45 * controller.armLevel);
  armR.write(-45 * controller.armLevel);
}