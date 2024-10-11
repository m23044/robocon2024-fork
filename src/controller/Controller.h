#pragma once

#include <liboshima.h>

// ロボットとの接続が成功したことを示す文字列
#define CONNECT_SUCCESS "!"

// モーターの数を定義
#define NUM_MOTORS 5

// ボタンの数を定義
#define NUM_BUTTONS 0

// スティックの数を定義
#define NUM_STICKS 0

// ControllerData テンプレートを使用して Controller 型を定義
using Controller = ControllerData<NUM_MOTORS, NUM_BUTTONS, NUM_STICKS>;