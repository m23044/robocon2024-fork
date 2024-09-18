// 他のファイルからプログラムを取得する
#include "../controller/Controller.h" // 一つ上のディレクトリのcontrollerディレクトリのController.hを取得
#include <Arduino.h>  // Arduinoの基本機能を使うためのライブラリ
#include <TimerOne.h> // タイマーを使うためのライブラリ
#include <components/ims/ImReceiver.h> // liboshima(大島商船用ライブラリ)のImReceiver.hを取得
#include <components/motors/BD62193.h>  // liboshimaのBD62193.hを取得
#include <components/motors/TB67H450.h> // liboshimaのTB67H450.hを取得

// 使用可能なピン:
// PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3
/*
  TB67H450型の変数を定義している。
  TB67H450型はint型と違い複雑な型であるため、int型の様に数字1つで初期化することはできない。

  int i = 1;
  でint型のi変数を作成し、1で初期化する。

  TB67H450 tireL = TB67H450(PIN_PD2,PIN_PD4);
  でTB67H450型のtireL変数を作成し、 PIN_PD2とPIN_PD4で初期化する。

  TB67H450はモータドライバの型番で、このモータドライバを扱うには、3つのピンが必要なのだ。
*/
TB67H450 tireL = TB67H450(PIN_PD2, PIN_PD4);
TB67H450 tireR = TB67H450(PIN_PD7, PIN_PB0);
TB67H450 holder = TB67H450(PIN_PC3, PIN_PC2);
TB67H450 puller = TB67H450(PIN_PC1, PIN_PC0);

// 緊急停止用の関数
void emergencyStop() {
  tireL.stop();
  tireR.stop();
  holder.stop();
  puller.stop();
}

// setup1回だけ実行される
void setup() {
  // 0.5秒のタイマーを設定
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS);  // Timer1.initialize(0.5);
  // タイマーが0.5秒経過したらemergencyStop関数を呼び出すように設定
  Timer1.attachInterrupt(emergencyStop);
  // タイマーをスタート
  Timer1.start();
}

void loop() {}

void serialEvent() {
  Timer1.restart(); // タイマーをリスタート
  Controller controller;
  static ImReceiver receiver(Serial); // 受信機の初期化 int a = 1;

  // コントローラーの変更を読み込む
  receiver.receive(controller);

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

  if (controller.hold) {
    holder.forward();
  } else if (controller.release) {
    holder.reverse();
  } else {
    holder.stop();
  }

  if (controller.pull) {
    puller.forward();
  } else if (controller.push) {
    puller.reverse();
  } else {
    puller.stop();
  }
}