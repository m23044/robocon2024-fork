#pragma once

#include <liboshima.h>

#define NUM_MOTORS 6
#define NUM_BUTTONS 0
#define NUM_STICKS 0

using Controller = ControllerData<NUM_MOTORS, NUM_BUTTONS, NUM_STICKS>;
