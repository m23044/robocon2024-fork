#pragma once

#include <liboshima.h>

#define CONNECT_SUCCESS "!" // ロボットとの接続が成功したことを示す文字列

#define NUM_MOTORS 5
#define NUM_BUTTONS 0
#define NUM_STICKS 0

using Controller = ControllerData<NUM_MOTORS, NUM_BUTTONS, NUM_STICKS>;
