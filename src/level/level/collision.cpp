#include <level/collision.hpp>

#include <objects/object.hpp>

Object *Collision::intersects(Object *obj) {
  for (auto const &target : objects_) {
    if (target->get_id() == obj->get_id()) {
      continue;
    }

    if (!target->is_active()) {
      continue;
    }

    if (target->get_position_x() == obj->get_position_x() &&
        target->get_position_y() == obj->get_position_y()) {
      target->on_collision(obj);
      obj->on_collision(target.get());
      return target.get();
    }
  }

  return nullptr;
}
