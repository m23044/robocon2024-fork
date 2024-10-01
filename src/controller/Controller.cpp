#include "Controller.h"

void Controller::set(bool state, uint8_t num) {
  if (state) {
    keyCode |= 1 << num;
  } else {
    keyCode &= ~(1 << num);
  }
}

bool Controller::get(uint8_t num) { return keyCode & (1 << num); }