#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体
struct Controller {
  unsigned int forwardL : 1; // 左側のモーターを前進させるボタン
  unsigned int reverseL : 1; // 左側のモーターを後退させるボタン
  unsigned int forwardR : 1; // 右側のモーターを前進させるボタン
  unsigned int reverseR : 1; // 右側のモーターを後退させるボタン
  unsigned int jump : 1;     // ジャンパーを動かすボタン
  unsigned int jumpSetUp : 1;  // ジャンパーを準備するボタン
  unsigned int shoot : 1;      // シューターを動かすボタン
  unsigned int shootSetUp : 1; // シューターを準備するボタン
};

#endif // CONTROLLER_H