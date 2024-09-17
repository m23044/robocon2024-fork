#ifndef ARM_BUTTONS_H
#define ARM_BUTTONS_H

struct ArmButtons {
  unsigned int armCatchL : 1;
  unsigned int armReleaseL : 1;
  unsigned int armCatchR : 1;
  unsigned int armReleaseR : 1;
};

#endif // ARM_BUTTONS_H