#pragma once

#include <memory>
#include <vector>

class Object;
class Level;

class Collision {
  friend class Level;

public:
  static Object *intersects(Object *obj);

private:
  static std::vector<std::shared_ptr<Object>> objects_;
};
