#include "ArmButtons.h"
#include "Controller.h"
#include <Arduino.h>
#include <components/ims/ImSender.h>

// 使用可能なピン: PD2, PD4, PD7, PB0, PC3, PC2, PC1, PC0
#define REVERSE_L_PIN PD2
#define FORWARD_L_PIN PD4
#define REVERSE_R_PIN PD7
#define FORWARD_R_PIN PB0
#define ARM_CATCH_PIN PC3
#define ARM_RELEASE_PIN PC2
#define JUMP_PIN PC1
#define JUMP_READY_PIN PC0

void setup() {
  pinMode(REVERSE_L_PIN, INPUT_PULLUP);
  pinMode(FORWARD_L_PIN, INPUT_PULLUP);
  pinMode(REVERSE_R_PIN, INPUT_PULLUP);
  pinMode(FORWARD_R_PIN, INPUT_PULLUP);
  pinMode(ARM_CATCH_PIN, INPUT_PULLUP);
  pinMode(ARM_RELEASE_PIN, INPUT_PULLUP);
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
  servoBtns.armCatch = !digitalRead(ARM_CATCH_PIN);
  servoBtns.armRelease = !digitalRead(ARM_RELEASE_PIN);

  static ArmButtons prevServoBtns;
  if (!prevServoBtns.armCatch && servoBtns.armCatch)
    controller.incrementArmLevel();
  else if (!prevServoBtns.armRelease && servoBtns.armRelease)
    controller.decrementArmLevel();

  static ImSender imSender(Serial);
  imSender.send(controller);
  prevServoBtns = servoBtns;
  delay(IM_SEND_INTERVAL);
}
