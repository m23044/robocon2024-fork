#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体
struct Controller {
  unsigned int forwardL : 1;
  unsigned int forwardR : 1;
  unsigned int reverseL : 1;
  unsigned int reverseR : 1;
  unsigned int armLevel : 3; // 0b111 = 7, 0b11 = 3
  unsigned int jump : 1;
  unsigned int jumpReady : 1;

  // サーボの値を0から3の範囲に制限するため
  void incrementArmLevel() {
    if (armLevel < 3)
      armLevel++;
  }
  void decrementArmLevel() {
    if (armLevel > 0)
      armLevel--;
  }
};

#endif // CONTROLLER_H