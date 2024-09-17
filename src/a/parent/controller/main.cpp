#include "Controller.h"
#include "components/ims/ImSender.h"
#include <Arduino.h>

// 使用可能なピン: PD3、PD4、PD5、PD6、PD7、PB0、PC0、PC1、PC2、PC3、PC4、PC5
#define FORWARD_L_PIN PD3
#define REVERSE_L_PIN PD4
#define FORWARD_R_PIN PD5
#define REVERSE_R_PIN PD6
#define JUMP_PIN PD7
#define JUMP_SET_UP_PIN PB0
#define SHOOT_PIN PC0
#define SHOOT_SET_UP_PIN PC1

// setup関数は、プログラムの開始時に一度だけ実行される。
void setup() {
  // 各ピンをINPUT_PULLUPに設定する。これにより、ピンがHIGHになる。
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  pinMode(JUMP_PIN, INPUT_PULLUP);
  pinMode(JUMP_SET_UP_PIN, INPUT_PULLUP);
  pinMode(SHOOT_PIN, INPUT_PULLUP);
  pinMode(SHOOT_SET_UP_PIN, INPUT_PULLUP);
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
  controller.shoot = !digitalRead(SHOOT_PIN);
  controller.shootSetUp = !digitalRead(SHOOT_SET_UP_PIN);

  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL);
}