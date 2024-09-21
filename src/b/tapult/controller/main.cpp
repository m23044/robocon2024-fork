#include "Controller.h"
#include <Arduino.h>
#include <components/ims/ImSender.h>
#include <digitalWriteFast.h>

// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
/*ロボットの制作状況に応に応じて有効にするか検討
#define FORWARD_L_PIN PIN_PD3
#define REVERSE_L_PIN PIN_PD4
#define FORWARD_R_PIN PIN_PD5
#define REVERSE_R_PIN PIN_PD6
*/
#define HOOK_PIN PIN_PD7
#define RELEASE_PIN PIN_PB0

void setup() {
  // 各ピンをINPUT_PULLUPに設定する。これにより、ピンがHIGHになる。
  /*ロボットの制作状況に応に応じて有効にするか検討
  pinModeFast(FORWARD_L_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_L_PIN, INPUT_PULLUP);
  pinModeFast(FORWARD_R_PIN, INPUT_PULLUP);
  pinModeFast(REVERSE_R_PIN, INPUT_PULLUP);
  */
  pinModeFast(HOOK_PIN, INPUT_PULLUP);
  pinModeFast(RELEASE_PIN, INPUT_PULLUP);
}

void loop() {
  static Controller controller;
  /*ロボットの制作状況に応に応じて有効にするか検討
  controller.forwardL = !digitalReadFast(FORWARD_L_PIN);
  controller.reverseL = !digitalReadFast(REVERSE_L_PIN);
  controller.forwardR = !digitalReadFast(FORWARD_R_PIN);
  controller.reverseR = !digitalReadFast(REVERSE_R_PIN);
  */
  controller.hook = !digitalReadFast(HOOK_PIN);
  controller.unhook = !digitalReadFast(RELEASE_PIN);

  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL);
}
