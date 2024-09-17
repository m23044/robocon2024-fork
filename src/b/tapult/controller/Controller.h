#ifndef CONTROLLER_H
#define CONTROLLER_H

struct Controller {
  unsigned int hook : 1;
  unsigned int unhook : 1;
  unsigned int forwardL : 1;
  unsigned int reverseL : 1;
  unsigned int forwardR : 1;
  unsigned int reverseR : 1;
}

#endif // CONTROLLER_H