#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体
struct Controller {
  unsigned int forwardL : 1;
  unsigned int reverseL : 1;
  unsigned int forwardR : 1;
  unsigned int reverseR : 1;  
  unsigned int jump : 1;
  unsigned int jumpReady : 1;
  unsigned int padding1 : 2; // 2ビットのパディング
  unsigned int armLevelL : 4; // 0b111 = 7, 0b11 = 3
  unsigned int armLevelR : 4;

  // サーボの値を0から5の範囲に制限するため
  void incrementArmLevelL() {
    if (armLevelL < 4)
      armLevelL++;
  }
  void decrementArmLevelL() {
    if (armLevelL > 0)
      armLevelL--;
  }

  void incrementArmLevelR() {
    if (armLevelR < 4)
      armLevelR++;
  }
  void decrementArmLevelR() {
    if (armLevelR > 0)
      armLevelR--;
  }
};

#endif // CONTROLLER_H