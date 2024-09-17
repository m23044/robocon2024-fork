// 多重インクルード防止のための定義
#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体（変数の集まり）
struct Controller {
  unsigned int hold : 1;     // ボールを咥えるボタン
  unsigned int release : 1;  // ボールを吐き出すボタン
  unsigned int push : 1;     // 首を引っ込めるボタン
  unsigned int pull : 1;     // 首を出すボタン
  unsigned int forwardL : 1; // 左タイヤを前進させるボタン
  unsigned int reverseL : 1; // 左タイヤを後退させるボタン
  unsigned int forwardR : 1; // 右タイヤを前進させるボタン
  unsigned int reverseR : 1; // 右タイヤを後退させるボタン
};

#endif // CONTROLLER_H