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
#define LOAD_PIN PIN_PD7
#define LOAD_SET_UP_PIN PIN_PB0
#define LAUNCH_PIN PIN_PB1
#define LAUNCH_SET_UP_PIN PIN_PB2

// setup関数は、プログラムの開始時に一度だけ実行される。
void setup() {
  // ピンのモードを設定する。INPUT_PULLUPは入力モードを意味する。
  pinModeFast(FORWARD_L_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_L_PIN, INPUT_PULLUP);
  pinModeFast(FORWARD_R_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_R_PIN, INPUT_PULLUP);
  pinModeFast(LOAD_PIN, INPUT_PULLUP);
  pinModeFast(LOAD_SET_UP_PIN, INPUT_PULLUP);
  pinModeFast(LAUNCH_PIN, INPUT_PULLUP);
  pinModeFast(LAUNCH_SET_UP_PIN, INPUT_PULLUP);
}

// loop関数は、プログラムが停止するまで繰り返し実行される。
void loop() {
  // Controllerオブジェクトを作成。
  Controller controller;

  // コントローラの状態を読み取る。
  controller.forwardL = !digitalReadFast(FORWARD_L_PIN);
  controller.reverseL = !digitalReadFast(REVERSE_L_PIN);
  controller.forwardR = !digitalReadFast(FORWARD_R_PIN);
  controller.reverseR = !digitalReadFast(REVERSE_R_PIN);
  controller.load = !digitalReadFast(LOAD_PIN);
  controller.loadSetUp = !digitalReadFast(LOAD_SET_UP_PIN);
  controller.launch = !digitalReadFast(LAUNCH_PIN);
  controller.launchSetUp = !digitalReadFast(LAUNCH_SET_UP_PIN);

  // 無条件でコントローラの状態を送信
  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL); // 60ms待機
}