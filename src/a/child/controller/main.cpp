// 他のファイルのプログラムを取得する
#include "Controller.h" // 同じディレクトリにあるController.hを取得
#include <Arduino.h> // Arduinoの基本機能を使うためのライブラリ
#include <components/ims/ImSender.h> // liboshima(大島商船用ライブラリ)のImSender.hを取得

// #defineでピン番号に別名をつける
// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
// PIN_PD3ピンをHOLD_PINとして定義（PD3は定数3を意味する）
#define FORWARD_L_PIN PIN_PD3
#define REVERSE_L_PIN PIN_PD4
#define FORWARD_R_PIN PIN_PD5
#define REVERSE_R_PIN PIN_PD6
#define HOLD_PIN PIN_PD7
#define RELEASE_PIN PIN_PB0
#define PUSH_PIN PIN_PB1
#define PULL_PIN PIN_PB2

// 1度だけ実行される
void setup() {
  // 各ピンに対しボタンとして使うための設定を行う
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  pinMode(HOLD_PIN, INPUT_PULLUP);
  pinMode(RELEASE_PIN, INPUT_PULLUP);
  pinMode(PUSH_PIN, INPUT_PULLUP);
  pinMode(PULL_PIN, INPUT_PULLUP);
}

// 繰り返し実行される
void loop() {
  // コントローラー変数を作成
  // int a; と同じ意味
  Controller controller;

  // コントローラーの状態を読み取る
  // controller変数は変数の集まりで、各変数には「.」を使ってアクセスする
  // 『!digitalRead(HOLD_PIN)』はボタンが押されている(いない)時に1(0)になる
  controller.forwardL = !digitalRead(FORWARD_L_PIN);
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
  controller.reverseR = !digitalRead(REVERSE_R_PIN);
  controller.hold = !digitalRead(HOLD_PIN);
  controller.release = !digitalRead(RELEASE_PIN);
  controller.push = !digitalRead(PUSH_PIN);
  controller.pull = !digitalRead(PULL_PIN);

  // IM920SLの送信機の変数を作成
  // staticはその変数が関数の呼び出しのたびに初期化されないようにする
  // つまりこの場合、sender変数は1度だけ初期化される
  static ImSender sender(Serial);
  // コントローラーの状態を送信
  sender.send(controller);
  // 60ミリ秒待機
  delay(IM_SEND_INTERVAL);
}