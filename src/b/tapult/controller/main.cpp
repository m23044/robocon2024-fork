#include "Controller.h"
#include <Arduino.h>
#include <components/ims/ImSender.h>

// 使用可能なピン: PD3、PD4、PD5、PD6、PD7、PB0、PC0、PC1、PC2、PC3、PC4、PC5
#define HOOK_PIN PD3
#define RELEASE_PIN PD4
/*
#define FORWARD_L_PIN PD5
#define REVERSE_L_PIN PD6
#define FORWARD_R_PIN PD7
#define REVERSE_R_PIN PB0
*/

void setup() {
  pinMode(HOOK_PIN, INPUT_PULLUP);
  pinMode(RELEASE_PIN, INPUT_PULLUP);
  /*
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  */
}

void loop() {
  static Controller controller;
  controller.hook = !digitalRead(HOOK_PIN);
  controller.unhook = !digitalRead(RELEASE_PIN);
  /*
  controller.forwardL = !digitalRead(FORWARD_L_PIN);
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
  controller.reverseR = !digitalRead(REVERSE_R_PIN);
  */

  static ImSender imSender(Serial);
  imSender.send(controller);
  delay(IM_SEND_INTERVAL);
}
