#include "Controller.h"
#include "components/ims/ImSender.h"
#include <Arduino.h>
#include <digitalWriteFast.h>

// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
#define FORWARD_L_PIN PIN_PD3
#define REVERSE_L_PIN PIN_PD4
#define FORWARD_R_PIN PIN_PD5
#define REVERSE_R_PIN PIN_PD6
#define JUMP_PIN PIN_PD7
#define JUMP_SET_UP_PIN PIN_PB0
#define SHOOT_PIN PIN_PB1
#define SHOOT_SET_UP_PIN PIN_PB2

// setup関数は、プログラムの開始時に一度だけ実行される。
void setup() {
  // 各ピンをINPUT_PULLUPに設定する。これにより、ピンがHIGHになる。
  pinModeFast(FORWARD_L_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_L_PIN, INPUT_PULLUP);
  pinModeFast(FORWARD_R_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_R_PIN, INPUT_PULLUP);
  pinModeFast(JUMP_PIN, INPUT_PULLUP);
  pinModeFast(JUMP_SET_UP_PIN, INPUT_PULLUP);
  pinModeFast(SHOOT_PIN, INPUT_PULLUP);
  pinModeFast(SHOOT_SET_UP_PIN, INPUT_PULLUP);
}

// loop関数は、プログラムが停止するまで繰り返し実行される。
void loop() {
  // Controllerオブジェクトを作成。これにより、コントローラの状態を管理する。
  Controller controller;

  // 各ピンの状態を読み取り、controllerオブジェクトに保存する
  controller.forwardL = !digitalReadFast(FORWARD_L_PIN);
  controller.reverseL = !digitalReadFast(REVERSE_L_PIN);
  controller.forwardR = !digitalReadFast(FORWARD_R_PIN);
  controller.reverseR = !digitalReadFast(REVERSE_R_PIN);
  controller.jump = !digitalReadFast(JUMP_PIN);
  controller.jumpSetUp = !digitalReadFast(JUMP_SET_UP_PIN);
  controller.shoot = !digitalReadFast(SHOOT_PIN);
  controller.shootSetUp = !digitalReadFast(SHOOT_SET_UP_PIN);

  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL);
}