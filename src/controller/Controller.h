#pragma once

#include <liboshima.h>

// モーターの数を定義
#define NUM_MOTORS 5

// モーター以外のボタンの数を定義
#define NUM_OTHER_BUTTONS 0

// スティックの数を定義
#define NUM_STICKS 0

// ControllerData テンプレートを使用して Controller 型を定義
using Controller = ControllerData<NUM_MOTORS, NUM_OTHER_BUTTONS, NUM_STICKS>;