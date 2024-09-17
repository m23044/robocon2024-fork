#include "Controller.h"
#include "components/ims/ImSender.h"
#include <Arduino.h>

// ピンの定義。これらはマイクロコントローラの特定のピン番号を表す。
#define FORWARD_L_PIN PIN_PD3      // 左側のタイヤの前進ピン
#define REVERSE_L_PIN PIN_PD4      // 左側のタイヤの後退ピン
#define FORWARD_R_PIN PIN_PD5      // 右側のタイヤの前進ピン
#define REVERSE_R_PIN PIN_PD6      // 右側のタイヤの後退ピン
#define JUMP_PIN PIN_PD7           // ジャンパーのピン
#define JUMP_SET_UP_PIN PIN_PB0    // ジャンパーの準備ピン
#define SHOOTER_PIN PIN_PB1        // シューターのピン
#define SHOOTER_SET_UP_PIN PIN_PB2 // シューターの準備ピン

// setup関数は、プログラムの開始時に一度だけ実行される。
void setup() {
  // 各ピンをINPUT_PULLUPに設定する。これにより、ピンがHIGHになる。
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  pinMode(JUMP_PIN, INPUT_PULLUP);
  pinMode(JUMP_SET_UP_PIN, INPUT_PULLUP);
  pinMode(SHOOTER_PIN, INPUT_PULLUP);
  pinMode(SHOOTER_SET_UP_PIN, INPUT_PULLUP);
}

// loop関数は、プログラムが停止するまで繰り返し実行される。
void loop() {
  // Controllerオブジェクトを作成。これにより、コントローラの状態を管理する。
  Controller controller;

  // 各ピンの状態を読み取り、controllerオブジェクトに保存する
  controller.forwardL = !digitalRead(FORWARD_L_PIN);
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
  controller.reverseR = !digitalRead(REVERSE_R_PIN);
  controller.jump = !digitalRead(JUMP_PIN);
  controller.jumpSetUp = !digitalRead(JUMP_SET_UP_PIN);
  controller.shoot = !digitalRead(SHOOTER_PIN);
  controller.shootSetUp = !digitalRead(SHOOTER_SET_UP_PIN);

  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL);
}