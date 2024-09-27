#pragma once

// コントローラーの構造体（変数の集まり）
struct Controller {
  unsigned int action1_1 : 1; // アクション1_1
  unsigned int action1_2 : 1; // アクション1_2
  unsigned int action2_1 : 1; // アクション2_1
  unsigned int action2_2 : 1; // アクション2_2
  unsigned int action3_1 : 1; // アクション3_1
  unsigned int action3_2 : 1; // アクション3_2
  unsigned int action4_1 : 1; // アクション4_1
  unsigned int action4_2 : 1; // アクション4_2
  unsigned int action5_1 : 1; // アクション5_1
  unsigned int action5_2 : 1; // アクション5_2
  unsigned int action6_1 : 1; // アクション6_1
  unsigned int action6_2 : 1; // アクション6_2
  unsigned int action7_1 : 1; // アクション7_1（未使用）
  unsigned int action7_2 : 1; // アクション7_2（未使用）
  unsigned int action8_1 : 1; // アクション8_1（未使用）
  unsigned int action8_2 : 1; // アクション8_2（未使用）
};
