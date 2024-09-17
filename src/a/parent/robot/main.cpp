#include "../controller/Controller.h"
#include <Arduino.h>
#include <TimerOne.h>
#include <components/ims/ImReceiver.h>
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>

TB67H450 tireL = TB67H450(PIN_PD2, PIN_PD4); // 左側のモーターを初期化
TB67H450 tireR = TB67H450(PIN_PD7, PIN_PB0); // 右側のモーターを初期化
BD62193 jumper = BD62193(PIN_PC3, PIN_PC2); // ジャンパーモーターを初期化
BD62193 shooter = BD62193(PIN_PC1, PIN_PC0); // シューターモーターを初期化

void emergencyStop() {
  tireL.stop();   // 左側のモーターを停止させる
  tireR.stop();   // 右側のモーターを停止させる
  jumper.stop();  // ジャンパーモーターを停止させる
  shooter.stop(); // シューターモーターを停止させる
}

void setup() {
  Timer1.initialize(500000);             // タイマーを初期化
  Timer1.attachInterrupt(emergencyStop); // タイマー割り込みを設定
}

void loop() {}

void serialEvent() {
  Timer1.restart(); // タイマーを再起動

  Controller controller; // コントローラーのインスタンスを作成
  // ImReceiverのインスタンスを作成
  static ImReceiver receiver(Serial);
  receiver.receive(controller); // コントローラーの状態を受信

  // コントローラーの状態に応じてモーターを制御
  if (controller.forwardL) {
    tireL.forward();
  } else if (controller.reverseL) {
    tireL.reverse();
  } else {
    tireL.stop();
  }

  if (controller.forwardR) {
    tireR.forward();
  } else if (controller.reverseR) {
    tireR.reverse();
  } else {
    tireR.stop();
  }

  if (controller.jump) {
    jumper.forward();
  } else if (controller.jumpSetUp) {
    jumper.reverse();
  } else {
    jumper.stop();
  }

  if (controller.shoot) {
    shooter.forward();
  } else if (controller.shootSetUp) {
    shooter.reverse();
  } else {
    shooter.stop();
  }
}