// 他のファイルのプログラムを取得する
#include "Controller.h" // 同じディレクトリにあるController.hを取得
#include <Arduino.h> // Arduinoの基本機能を使うためのライブラリ
#include <TimerOne.h>
#include <components/ims/IM920SL.h> // liboshima(大島商船用ライブラリ)のIM920SL.hを取得
#include <digitalWriteFast.h> // digitalWriteFast.hを取得

// #defineでピン番号に別名をつける
// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
// PIN_PD3ピンをHOLD_PINとして定義（PD3は定数3を意味する）
#define ACTION1_1_PIN PIN_PD3
#define ACTION1_2_PIN PIN_PD4
#define ACTION2_1_PIN PIN_PD5
#define ACTION2_2_PIN PIN_PD6
#define ACTION3_1_PIN PIN_PD7
#define ACTION3_2_PIN PIN_PB0
#define ACTION4_1_PIN PIN_PB1
#define ACTION4_2_PIN PIN_PB2
#define ACTION5_1_PIN PIN_PC0
#define ACTION5_2_PIN PIN_PC1
#define EMERGENCY_LED_PIN PIN_PC2
SerialPort serial(Serial); // シリアル通信の設定
IM920SL im(serial);        // ImSender型のsender変数を宣言する

// 緊急停止のランプを点灯させる関数
void emergencyStop() {
  digitalWriteFast(EMERGENCY_LED_PIN, HIGH); // ランプを点灯
}

// 1度だけ実行される
void setup() {
  // 各ピンに対しボタンとして使うための設定を行う
  pinModeFast(ACTION1_1_PIN, INPUT_PULLUP);
  pinModeFast(ACTION1_2_PIN, INPUT_PULLUP);
  pinModeFast(ACTION2_1_PIN, INPUT_PULLUP);
  pinModeFast(ACTION2_2_PIN, INPUT_PULLUP);
  pinModeFast(ACTION3_1_PIN, INPUT_PULLUP);
  pinModeFast(ACTION3_2_PIN, INPUT_PULLUP);
  pinModeFast(ACTION4_1_PIN, INPUT_PULLUP);
  pinModeFast(ACTION4_2_PIN, INPUT_PULLUP);
  pinModeFast(ACTION5_1_PIN, INPUT_PULLUP);
  pinModeFast(ACTION5_2_PIN, INPUT_PULLUP);
  pinModeFast(EMERGENCY_LED_PIN, OUTPUT); // ランプを出力に設定
  im.begin();                             // IM920SLの初期化
  // 1秒のタイマーを設定
  Timer1.initialize(IM_RECEIVE_INTERVAL_MICROS);
  // タイマーが1秒経過したらemergencyStop関数を呼び出すように設定
  Timer1.attachInterrupt(emergencyStop);
  Timer1.start(); // タイマーをスタート
}

// 繰り返し実行される
void loop() {
  // コントローラー変数を作成
  // int a; と同じ意味
  Controller controller; // Controller複数の変数でできている

  // コントローラーの状態を読み取る
  // controller変数は変数の集まりで、各変数には「.」を使ってアクセスする
  // 『!digitalReadFast(ACTIONn_n_PIN)』はボタンが押されている(いない)時に1(0)になる
  controller.action1_1 = !digitalReadFast(ACTION1_1_PIN);
  controller.action1_2 = !digitalReadFast(ACTION1_2_PIN);
  controller.action2_1 = !digitalReadFast(ACTION2_1_PIN);
  controller.action2_2 = !digitalReadFast(ACTION2_2_PIN);
  controller.action3_1 = !digitalReadFast(ACTION3_1_PIN);
  controller.action3_2 = !digitalReadFast(ACTION3_2_PIN);
  controller.action4_1 = !digitalReadFast(ACTION4_1_PIN);
  controller.action4_2 = !digitalReadFast(ACTION4_2_PIN);
  controller.action5_1 = !digitalReadFast(ACTION5_1_PIN);
  controller.action5_2 = !digitalReadFast(ACTION5_2_PIN);

  // コントローラーの状態を送信
  im.send(controller); // imでコントローラーの状態を送信している
  // 60ミリ秒待機
  delay(IM_SEND_INTERVAL); // delay(60);
}

// シリアル通信があった時に実行される
void serialEvent() {
  digitalWriteFast(EMERGENCY_LED_PIN, LOW); // ランプを点灯
  Timer1.restart();                         // タイマーをリスタート
}