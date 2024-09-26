// 他のファイルのプログラムを取得する
#include "Controller.h" // 同じディレクトリにあるController.hを取得
#include <Arduino.h>
#include <MsTimer2.h> // MsTimer2.hを取得
#include <components/ims/IM920SL.h> // liboshima(大島商船用ライブラリ)のIM920SL.hを取得
#include <digitalWriteFast.h> // digitalWriteFast.hを取得

// #defineでピン番号に別名をつける
// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
#define ACTION1_1_PIN PIN_PD3 // PIN_PD3ピンをACTION1_1_PINとして定義
#define ACTION1_2_PIN PIN_PD4
#define ACTION2_1_PIN PIN_PD5
#define ACTION2_2_PIN PIN_PD6
#define ACTION3_1_PIN PIN_PD7
#define ACTION3_2_PIN PIN_PB0
#define ACTION4_1_PIN PIN_PB1
#define ACTION4_2_PIN PIN_PB2
#define ACTION5_1_PIN PIN_PC2
#define ACTION5_2_PIN PIN_PC1
#define CONNECT_LED_PIN PIN_PC0 // 受信中のランプのピン番号

// SerialPort型のserial変数を宣言し、Serial変数で初期化する
// やっていることは、int a = 1; と同じ
// イメージ：SerialPort serial = Serial;
SerialPort serial(Serial);

// ImSender型のsender変数を宣言し、serial変数で初期化する
// やっていることは、int a = 1; と同じ
// イメージ：IM920SL im = serial;
IM920SL im(serial);

// 緊急停止のランプを点灯させる関数
// 500ミリ秒経過すると、Timer1によって呼び出される
void emergencyStop() {
  digitalWriteFast(CONNECT_LED_PIN, HIGH); // ランプを点灯
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
  // ランプを出力に設定
  pinModeFast(CONNECT_LED_PIN, OUTPUT);

  // IM920SLの初期化
  im.begin();

  // 500000マイクロ秒(0.5秒)のタイマーを設定
  MsTimer2::set(IM_RECEIVE_INTERVAL_MILLIS, emergencyStop);
  MsTimer2::start();
}

// 繰り返し実行される
void loop() {
  // コントローラー変数を作成
  // int a; と同じ意味
  Controller controller;

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

  // コントローラーの状態をIM920SLを使って送信
  im.send(controller);
  // 100ミリ秒待機
  delay(IM_SEND_INTERVAL);
}

// ロボットから返答があった時に呼び出される関数
void serialEvent() {
  // ランプを消灯
  digitalWriteFast(CONNECT_LED_PIN, LOW);
  // タイマーのカウントを最初からやり直す
  MsTimer2::start();
}
