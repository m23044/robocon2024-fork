#include "../controller/Controller.h"
#include <Arduino.h>
#include <TimerOne.h>
#include <components/ims/ImReceiver.h>
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>
#include <components/servos/MG90S.h>

#define HOOK_DEGREE 45

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PC1, PC0
TB67H450 tireL = TB67H450(PIN_PD2, PIN_PD4); // 左側のモーターを初期化
TB67H450 tireR = TB67H450(PIN_PD7, PIN_PB0); // 右側のモーターを初期化
MG90S hooker;                                // フックを初期化

int currentHookDegree = HOOK_DEGREE;

void emergencyStop() {
  tireL.stop(); // 左側のモーターを停止させる
  tireR.stop(); // 右側のモーターを停止させる
}

void setup() {
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS); // タイマーを初期化
  Timer1.attachInterrupt(emergencyStop); // タイマー割り込みを設定
  Timer1.start();                        // タイマーを開始
  hooker.attach(PIN_PC3);                // フックをPC3に接続
}

void loop() { hooker.write(currentHookDegree); }

void serialEvent() {
  Timer1.restart(); // タイマーを再起動

  // ImReceiverのインスタンスを作成
  Controller controller;
  static ImReceiver receiver(Serial);
  receiver.receive(controller); // コントローラーの状態を受信

  /*
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
  */

  if (controller.hook) {
    currentHookDegree = HOOK_DEGREE;
  } else if (controller.unhook) {
    currentHookDegree = 0;
  }
}