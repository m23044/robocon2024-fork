// 他のファイルからプログラムを取得する
#include "controller/Controller.h" // 一つ上のディレクトリのcontrollerディレクトリのController.hを取得
#include <Arduino.h>               // Arduino.hを取得
#include <Servo.h>    // サーボモータを使うためのライブラリ
#include <TimerOne.h> // タイマーを使うためのライブラリ
#include <components/ims/IM920SL.h> // liboshima(大島商船用ライブラリ)のIM920SL.hを取得
#include <components/motors/NonSpeedAdjustable.h> // liboshimaのNonSpeedAdjustable.hを取得

// もしBチームのロボット2のビルドを行う場合は以下のコードを有効にする
#if defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
#define ONE_LEVEL_DEGREE 45 // サーボモーターの1段階の角度を45度に設定
#endif

/*
  NonSpeedAdjustable型の変数を定義している。
  NonSpeedAdjustable型はint型と違い複雑な型であるため、int型の様に数字1つで初期化することはできない。

  int i = 1;
  でint型のi変数を作成し、1で初期化する。

  NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
  でNonSpeedAdjustable型のaction1変数を作成し、PIN_PD2とPIN_PD4で初期化する。

  ※NonSpeedAdjustableはモータドライバのことで、2つのピンを指定することでモータを制御することができる。※
*/
// 使用可能なピン:
// タパルト：PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC2、PIN_PC3、[PIN_PB1、PIN_PB2]
// その他　：PIN_PD2、PIN_PD4、PIN_PD7、PIN_PB0、PIN_PC2、PIN_PC3、[PIN_PC0、PIN_PC1]
// もしBチームのロボット2のビルドを行う場合は以下のコードを有効にする
#if defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
NonSpeedAdjustable action2(PIN_PD7, PIN_PB0);
NonSpeedAdjustable action3(PIN_PC3, PIN_PC2);
Servo action4;
Servo action5;
// もしBチームのロボット2のビルドを行なわない場合は以下のコードを有効にする
#else
NonSpeedAdjustable action1(PIN_PD2, PIN_PD4);
NonSpeedAdjustable action2(PIN_PD7, PIN_PB0);
NonSpeedAdjustable action3(PIN_PC3, PIN_PC2);
NonSpeedAdjustable action4(PIN_PC1, PIN_PC0);
#endif

SerialPort serial(Serial); // SerialPort serial = Serial;
IM920SL im(serial);        // IM920SL im = serial;
Controller controller;     // int a;

// もしBチームのロボット2のビルドを行う場合は以下のコードを有効にする
#if defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
uint8_t level1 = 0;
uint8_t level2 = 0;
#endif

// 緊急停止用の関数
// 500ミリ秒経過すると、Timer1によって呼び出される
void emergencyStop() {
  action1.stop();
  action2.stop();
  action3.stop();

  // もしBチームのロボット2のビルドを行なわない場合は以下のコードを有効にする
#if !defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
  action4.stop();
#endif
}

// setup1回だけ実行される
void setup() {
  // 500000マイクロ秒(0.5秒)のタイマーを設定
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS); // Timer1.initialize(500000);
  // タイマーが0.5秒経過したらemergencyStop関数を呼び出すように設定
  Timer1.attachInterrupt(emergencyStop);
  // タイマーをスタート
  Timer1.start();

  // IM920SLの初期化
  im.begin();

  // もしBチームのロボット2のビルドを行う場合は以下のコードを有効にする
#if defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
  action4.attach(PIN_PB1); // サーボモータ1をPIN_PB1に接続
  action5.attach(PIN_PB2); // サーボモータ2をPIN_PB2に接続
#endif
}

void loop() {
#if defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
  action4.write(ONE_LEVEL_DEGREE * level1);
  action5.write(ONE_LEVEL_DEGREE * level2);
#endif
}

void serialEvent() {
  Timer1.restart(); // タイマーをリスタート

  // コントローラーの変更を読み込む
  im.receive(controller);

  // もしaction1_1ボタンが押されていたら
  if (controller.action1_1) {
    // action1を前進させる
    action1.forward();
  }
  // もしaction1_2ボタンが押されていたら
  else if (controller.action1_2) {
    // action1を後進させる
    action1.reverse();
  }
  // それ以外の場合
  else {
    // action1を停止させる
    action1.stop();
  }

  // もしaction2_1ボタンが押されていたら
  if (controller.action2_1) {
    // action2を前進させる
    action2.forward();
  }
  // もしaction2_2ボタンが押されていたら
  else if (controller.action2_2) {
    // action2を後進させる
    action2.reverse();
  }
  // それ以外の場合
  else {
    // action2を停止させる
    action2.stop();
  }

  // もしaction3_1ボタンが押されていたら
  if (controller.action3_1) {
    // action3を前進させる
    action3.forward();
  }
  // もしaction3_2ボタンが押されていたら
  else if (controller.action3_2) {
    // action3を後進させる
    action3.reverse();
  }
  // それ以外の場合
  else {
    // action3を停止させる
    action3.stop();
  }

  // もしBチームのロボット2のビルドを行なわない場合は以下のコードを有効にする
#if !defined(ROBOCCHI) // ROBOCCHIはplatformio.iniの57行目で定義されている
  // もしaction4_1ボタンが押されていたら
  if (controller.action4_1) {
    // action4を前進させる
    action4.forward();
  }
  // もしaction4_2ボタンが押されていたら
  else if (controller.action4_2) {
    // action4を後進させる
    action4.reverse();
  }
  // それ以外の場合
  else {
    // action4を停止させる
    action4.stop();
  }

  // もしBチームのロボット2のビルドを行う場合は以下のコードを有効にする
#else
  // Controller型のprevController変数を定義
  // staticキーワードによって、その変数が関数の呼び出しのたびに初期化されないようにする
  static Controller prevController;

  // もしaction4_1ボタンが新たに押さ、かつサーボモーターの段階が4未満の場合
  if (!prevController.action4_1 && controller.action4_1 && level1 < 4) {
    // サーボモーターの段階を1段階上げる
    level1++;
  }
  // もしaction4_2ボタンが新たに押さ、かつサーボモーターの段階が0より大きい場合
  else if (!prevController.action4_2 && controller.action4_2 && level1 > 0) {
    // サーボモーターの段階を1段階下げる
    level1--;
  }
  // もしaction5_1ボタンが新たに押さ、かつサーボモーターの段階が4未満の場合
  if (!prevController.action5_1 && controller.action5_1 && level2 < 4) {
    // サーボモーターの段階を1段階上げる
    level2++;
  }
  // もしaction5_2ボタンが新たに押さ、かつサーボモーターの段階が0より大きい場合
  else if (!prevController.action5_2 && controller.action5_2 && level2 > 0) {
    // サーボモーターの段階を1段階下げる
    level2--;
  }

  // prevController変数をcontroller変数で更新
  prevController = controller;
#endif

  // コントローラーに受信が完了したことを知らせる
  im.send("Succes");
  //Serial.println("TXDA 1111");
}
