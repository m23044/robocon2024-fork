#pragma once

#include <components/controllers/ControllerData.h>

#define NUM_MOTORS 5
#define NUM_BUTTONS 0
#define NUM_STICKS 0

using Controller = ControllerData<NUM_MOTORS, NUM_BUTTONS, NUM_STICKS>;
