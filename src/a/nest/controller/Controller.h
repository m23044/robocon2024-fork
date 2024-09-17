#ifndef CONTROLLER_H
#define CONTROLLER_H

// コントローラーの構造体
struct Controller {
  unsigned int forwardL : 1; // 左側のモーターを前進させるボタン
  unsigned int reverseL : 1; // 左側のモーターを後退させるボタン
  unsigned int forwardR : 1; // 右側のモーターを前進させるボタン
  unsigned int reverseR : 1; // 右側のモーターを後退させるボタン
  unsigned int load : 1; // モーターを回転させてボールをロードするボタン
  unsigned int loadSetUp : 1; // モーターを回転させてボールをセットアップするボタン
  unsigned int launch : 1; // モーターを回転させてボールを発射するボタン
  unsigned int launchSetUp : 1; // モーターを回転させてボールをセットアップするボタン
};

#endif // CONTROLLER_H