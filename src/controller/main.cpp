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
#define BTN1_PIN PIN_PD3 // PIN_PD3ピンをBTN1_PINとして定義
#define BTN2_PIN PIN_PD4
#define BTN3_PIN PIN_PD5
#define BTN4_PIN PIN_PD6
#define BTN5_PIN PIN_PD7
#define BTN6_PIN PIN_PB0
#define BTN7_PIN PIN_PB1
#define BTN8_PIN PIN_PB2
#define BTN9_PIN PIN_PC2
#define BTN10_PIN PIN_PC3
#define BTN11_PIN PIN_PC4
#define BTN12_PIN PIN_PC5
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
  pinModeFast(BTN1_PIN, INPUT_PULLUP);
  pinModeFast(BTN2_PIN, INPUT_PULLUP);
  pinModeFast(BTN3_PIN, INPUT_PULLUP);
  pinModeFast(BTN4_PIN, INPUT_PULLUP);
  pinModeFast(BTN5_PIN, INPUT_PULLUP);
  pinModeFast(BTN6_PIN, INPUT_PULLUP);
  pinModeFast(BTN7_PIN, INPUT_PULLUP);
  pinModeFast(BTN8_PIN, INPUT_PULLUP);
  pinModeFast(BTN9_PIN, INPUT_PULLUP);
  pinModeFast(BTN10_PIN, INPUT_PULLUP);
  pinModeFast(BTN11_PIN, INPUT_PULLUP);
  pinModeFast(BTN12_PIN, INPUT_PULLUP);
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
  controller.btn1 = !digitalReadFast(BTN1_PIN);
  controller.btn2 = !digitalReadFast(BTN2_PIN);
  controller.btn3 = !digitalReadFast(BTN3_PIN);
  controller.btn4 = !digitalReadFast(BTN4_PIN);
  controller.btn5 = !digitalReadFast(BTN5_PIN);
  controller.btn6 = !digitalReadFast(BTN6_PIN);
  controller.btn7 = !digitalReadFast(BTN7_PIN);
  controller.btn8 = !digitalReadFast(BTN8_PIN);
  controller.btn9 = !digitalReadFast(BTN9_PIN);
  controller.btn10 = !digitalReadFast(BTN10_PIN);
  controller.btn11 = !digitalReadFast(BTN11_PIN);
  controller.btn12 = !digitalReadFast(BTN12_PIN);

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
