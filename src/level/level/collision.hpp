#pragma once

#include <memory>
#include <vector>

class Object;
class Level;

using Objects = std::vector<std::shared_ptr<Object>>;

class Collision {
  friend class Level;

public:
  static Object *intersects(Object *obj);

private:
  static std::shared_ptr<Objects> objects_;
};
