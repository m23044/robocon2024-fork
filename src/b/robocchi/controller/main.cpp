#include "ArmButtons.h"
#include "Controller.h"
#include <Arduino.h>
#include <components/ims/ImSender.h>

// 使用可能なピン定数；
// PIN_PD3、PIN_PD4、PIN_PD5、PIN_PD6、PIN_PD7、PIN_PB0、PIN_PB1、PIN_PB2、
// PIN_PC0、PIN_PC1、PIN_PC2、PIN_PC3、PIN_PC4、PIN_PC5
#define FORWARD_L_PIN PIN_PD3
#define REVERSE_L_PIN PIN_PD4
#define FORWARD_R_PIN PIN_PD5
#define REVERSE_R_PIN PIN_PD6
#define ARM_CATCH_L_PIN PIN_PD7
#define ARM_RELEASE_L_PIN PIN_PB0
#define ARM_CATCH_R_PIN PIN_PB1
#define ARM_RELEASE_R_PIN PIN_PB2
#define JUMP_PIN PIN_PC0
#define JUMP_READY_PIN PIN_PC1

void setup() {
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
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
  controller.reverseL = !digitalRead(REVERSE_L_PIN);
  controller.forwardR = !digitalRead(FORWARD_R_PIN);
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
