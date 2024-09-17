#include "../controller/Controller.h" // Controllerクラスのヘッダーファイルをインクルード
#include <Arduino.h> // Arduinoの標準ライブラリをインクルード
#include <TimerOne.h>
#include <components/ims/ImReceiver.h> // ImReceiverクラスのヘッダーファイルをインクルード
#include <components/motors/BD62193.h>
#include <components/motors/TB67H450.h>

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PC1, PC0
BD62193 caterpillarL(PD2, PD4);
BD62193 caterpillarR(PD7, PB0);
TB67H450 launcher(PC3, PC2);
TB67H450 loader(PC1, PC0);

void emergencyStop() {
  caterpillarL.stop();
  caterpillarR.stop();
  launcher.stop();
  loader.stop();
}

void setup() {
  Timer1.initialize(500000); // 0.5秒ごとに割り込みを発生させる
  Timer1.attachInterrupt(emergencyStop); // 割り込みハンドラを設定
  Timer1.start();                        // 割り込みを開始
}

void loop() {
  // メインの処理があればここに記述
}

void serialEvent() {
  Timer1.restart();

  // コントローラーの状態を受信
  Controller controller; // コントローラーのインスタンスを作成
  // シリアル通信のインスタンスを作成
  static ImReceiver receiver(Serial);
  receiver.receive(controller); // コントローラーの状態を受信

  // モーターを制御
  if (controller.forwardL) {
    caterpillarL.forward();
  } else if (controller.reverseL) {
    caterpillarL.reverse();
  } else {
    caterpillarL.stop();
  }

  if (controller.forwardR) {
    caterpillarR.forward();
  } else if (controller.reverseR) {
    caterpillarR.reverse();
  } else {
    caterpillarR.stop();
  }

  if (controller.load) {
    loader.forward();
  } else if (controller.loadSetUp) {
    loader.reverse();
  } else {
    loader.stop();
  }

  if (controller.launch) {
    launcher.forward();
  } else if (controller.launchSetUp) {
    launcher.reverse();
  } else {
    launcher.stop();
  }
}