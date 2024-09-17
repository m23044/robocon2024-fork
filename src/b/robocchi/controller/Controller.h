#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体
struct Controller {
  unsigned int forwardL : 1;
  unsigned int forwardR : 1;
  unsigned int reverseL : 1;
  unsigned int reverseR : 1;
  unsigned int armLevelL : 3; // 0b111 = 7, 0b11 = 3
  unsigned int armLevelR : 3;
  unsigned int jump : 1;
  unsigned int jumpReady : 1;

  // サーボの値を0から3の範囲に制限するため
  void incrementArmLevelL() {
    if (armLevelL < 3)
      armLevelL++;
  }
  void decrementArmLevelL() {
    if (armLevelL > 0)
      armLevelL--;
  }

  void incrementArmLevelR() {
    if (armLevelR < 3)
      armLevelR++;
  }
  void decrementArmLevelR() {
    if (armLevelR > 0)
      armLevelR--;
  }
};

#endif // CONTROLLER_H