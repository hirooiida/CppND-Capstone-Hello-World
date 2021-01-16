#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "vector2.h"

class Controller {
 public:
  void HandleInput(bool &running, Vector2 &dir);
};

#endif
