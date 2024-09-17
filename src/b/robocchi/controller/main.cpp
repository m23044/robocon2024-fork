#include "ArmButtons.h"
#include "Controller.h"
#include <Arduino.h>
#include <components/ims/ImSender.h>

// 使用可能なピン: PD3、PD4、PD5、PD6、PD7、PB0、PC0、PC1、PC2、PC3、PC4、PC5
#define REVERSE_L_PIN PD3
#define FORWARD_L_PIN PD4
#define REVERSE_R_PIN PD5
#define FORWARD_R_PIN PD6
#define ARM_CATCH_L_PIN PD7
#define ARM_RELEASE_L_PIN PB0
#define ARM_CATCH_R_PIN PC0
#define ARM_RELEASE_R_PIN PC1
#define JUMP_PIN PC2
#define JUMP_READY_PIN PC3

void setup() {
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(ARM_CATCH_L_PIN, INPUT_PULLUP);
  pinMode(ARM_RELEASE_L_PIN, INPUT_PULLUP);
  pinMode(ARM_CATCH_R_PIN, INPUT_PULLUP);
  pinMode(ARM_RELEASE_R_PIN, INPUT_PULLUP);
  pinMode(JUMP_PIN, INPUT_PULLUP);
  pinMode(JUMP_READY_PIN, INPUT_PULLUP);
}

void loop() {
  static Controller controller;
  controller.forwardL = !digitalRead(FORWARD_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.reverseR = !digitalRead(REVERSE_R_PIN);
  controller.jump = !digitalRead(JUMP_PIN);
  controller.jumpReady = !digitalRead(JUMP_READY_PIN);

  ArmButtons servoBtns;
  servoBtns.armCatchL = !digitalRead(ARM_CATCH_L_PIN);
  servoBtns.armReleaseL = !digitalRead(ARM_RELEASE_L_PIN);
  servoBtns.armCatchR = !digitalRead(ARM_CATCH_R_PIN);
  servoBtns.armReleaseR = !digitalRead(ARM_RELEASE_R_PIN);

  static ArmButtons prevServoBtns;
  if (!prevServoBtns.armCatchL && servoBtns.armCatchL)
    controller.incrementArmLevelL();
  else if (!prevServoBtns.armReleaseL && servoBtns.armReleaseL)
    controller.decrementArmLevelL();
  if (!prevServoBtns.armCatchR && servoBtns.armCatchR)
    controller.incrementArmLevelR();
  else if (!prevServoBtns.armReleaseR && servoBtns.armReleaseR)
    controller.decrementArmLevelR();

  static ImSender imSender(Serial);
  imSender.send(controller);
  prevServoBtns = servoBtns;
  delay(IM_SEND_INTERVAL);
}
