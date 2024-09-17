#include "Controller.h"
#include "components/ims/ImSender.h"
#include <Arduino.h>

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PC1, PC0
#define PULL_PIN PD2
#define LOOSEN_MOTOR_PIN PD4
#define FORWARD_L_PIN PD7
#define REVERSE_L_PIN PB0
#define FORWARD_R_PIN PC3
#define REVERSE_R_PIN PC2

// setup関数は、プログラムの開始時に一度だけ実行される。
void setup() {
  // ピンのモードを設定する。INPUT_PULLUPは入力モードを意味する。
  pinMode(PULL_PIN,
          INPUT_PULLUP); // ゴムを引っ張るモーターを入力モードに設定
  pinMode(LOOSEN_MOTOR_PIN,
          INPUT_PULLUP); // ゴムを緩めるモーターを入力モードに設定
  pinMode(FORWARD_L_PIN, INPUT_PULLUP); // FORWARD_L_PINを入力モードに設定
  pinMode(REVERSE_L_PIN, INPUT_PULLUP); // REVERSE_L_PINを入力モードに設定
  pinMode(FORWARD_R_PIN, INPUT_PULLUP); // FORWARD_R_PINを入力モードに設定
  pinMode(REVERSE_R_PIN, INPUT_PULLUP); // REVERSE_R_PINを入力モードに設定
}

// loop関数は、プログラムが停止するまで繰り返し実行される。
void loop() {
  // Controllerオブジェクトを作成。
  Controller controller;

  // コントローラの状態を読み取る。
  controller.forwardL = !digitalRead(FORWARD_L_PIN);
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
  controller.reverseR = !digitalRead(REVERSE_R_PIN);
  controller.load = !digitalRead(PULL_PIN);
  controller.loadSetUp = !digitalRead(LOOSEN_MOTOR_PIN);
  controller.launch = !digitalRead(PULL_PIN);
  controller.launchSetUp = !digitalRead(LOOSEN_MOTOR_PIN);

  // 無条件でコントローラの状態を送信
  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL); // 60ms待機
}