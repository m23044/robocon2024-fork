#include "../controller/Controller.h"
#include <Arduino.h>
#include <Servo.h>
#include <TimerOne.h>
#include <components/ims/ImReceiver.h>
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>

#define ONE_LEVEL_DEGREE 45

Controller controller;

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PB1, PB2
Servo armL;
Servo armR;
TB67H450 motorL(PD2, PD4);
TB67H450 motorR(PD7, PB0);
BD62193 jumpper(PC3, PC2);

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
  armL.attach(PB1);                      // サーボモータLをPB1に接続
  armR.attach(PB2);                      // サーボモータRをPB2に接続
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