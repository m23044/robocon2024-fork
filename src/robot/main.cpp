// 他のファイルからプログラムを取得する
#include "controller/Controller.h" // 一つ上のディレクトリのcontrollerディレクトリのController.hを取得
#include <Arduino.h>  // Arduinoの基本機能を使うためのライブラリ
#include <Servo.h>    // サーボモータを使うためのライブラリ
#include <TimerOne.h> // タイマーを使うためのライブラリ
#include <components/ims/IM920SL.h> // liboshima(大島商船用ライブラリ)のIM920SL.hを取得
#include <components/motors/NonSpeedAdjustable.h> // liboshimaのNonSpeedAdjustable.hを取得

#if defined(TAPULT)
#define ONE_LEVEL_DEGREE 45
#endif

/*
  TB67H450型の変数を定義している。
  TB67H450型はint型と違い複雑な型であるため、int型の様に数字1つで初期化することはできない。

  int i = 1;
  でint型のi変数を作成し、1で初期化する。

  TB67H450 tireL = TB67H450(PIN_PD2,PIN_PD4);
  でTB67H450型のtireL変数を作成し、 PIN_PD2とPIN_PD4で初期化する。

  TB67H450はモータドライバの型番で、このモータドライバを扱うには、2つのピンが必要なのだ。
*/
// 使用可能なピン:
// タパルト：PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC2、PIN_PC3、PIN_PB1、PIN_PB2
// その他　：PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC2、PIN_PC3、PIN_PC0、PIN_PC1
#if defined(TAPULT)
NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
NonSpeedAdjustable action2(PIN_PD7, PIN_PB0);
NonSpeedAdjustable action3(PIN_PC3, PIN_PC2);
Servo action4;
Servo action5;
#else
NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
NonSpeedAdjustable action2(PIN_PD7, PIN_PB0);
NonSpeedAdjustable action3(PIN_PC3, PIN_PC2);
NonSpeedAdjustable action4(PIN_PC1, PIN_PC0);
#endif

SerialPort serial(Serial); // シリアル通信の設定
IM920SL im(serial);        // 受信機の初期化 int a = 1;
Controller controller;     // Controller型のcontroller変数を宣言する

#if defined(TAPULT)
uint8_t level1 = 0;
uint8_t level2 = 0;
#endif

// 緊急停止用の関数
void emergencyStop() {
  action1.stop();
  action2.stop();
  action3.stop();
#if not defined(TAPULT)
  action4.stop();
#endif
}

// setup1回だけ実行される
void setup() {
  // 0.5秒のタイマーを設定
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS); // Timer1.initialize(0.5);
  // タイマーが0.5秒経過したらemergencyStop関数を呼び出すように設定
  Timer1.attachInterrupt(emergencyStop);
  // タイマーをスタート
  Timer1.start();
  im.begin(); // IM920SLの初期化
#if defined(TAPULT)
  action4.attach(PIN_PB1); // サーボモータ1をPIN_PB1に接続
  action5.attach(PIN_PB2); // サーボモータ2をPIN_PB2に接続
#endif
}

void loop() {
#if defined(TAPULT)
  action4.write(ONE_LEVEL_DEGREE * level1);
  action5.write(ONE_LEVEL_DEGREE * level2);
#endif
}

void serialEvent() {
  Timer1.restart(); // タイマーをリスタート

  // コントローラーの変更を読み込む
  im.receive(controller);

  // コントローラーの状態に応じてモーターを制御
  if (controller.action1_1) {
    action1.forward();
  } else if (controller.action1_2) {
    action1.reverse();
  } else {
    action1.stop();
  }

  if (controller.action2_1) {
    action2.forward();
  } else if (controller.action2_2) {
    action2.reverse();
  } else {
    action2.stop();
  }

  if (controller.action3_1) {
    action3.forward();
  } else if (controller.action3_2) {
    action3.reverse();
  } else {
    action3.stop();
  }

#if not defined(TAPULT)
  if (controller.action4_1) {
    action4.forward();
  } else if (controller.action4_2) {
    action4.reverse();
  } else {
    action4.stop();
  }
#else
  static Controller prevController;
  if (!prevController.action4_1 && controller.action4_1 && level1 < 4) {
    level1++;
  } else if (!prevController.action4_2 && controller.action4_2 && level1 > 0) {
    level1--;
  }
  if (!prevController.action5_1 && controller.action5_1 && level2 < 4) {
    level2++;
  } else if (!prevController.action5_2 && controller.action5_2 && level2 > 0) {
    level2--;
  }
  prevController = controller;
#endif

  im.send("Success");
}