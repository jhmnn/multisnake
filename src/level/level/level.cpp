#include <level/level.hpp>

#include <level/collision.hpp>
#include <objects/object.hpp>

Level::Level() {
  objects_ = std::make_shared<Objects>();
  Collision::objects_ = objects_;
}

void Level::update() {
  for (auto const &obj : *objects_) {
    if (!obj->is_active()) {
      continue;
    }

    obj->update();
  }
}

void Level::draw() const {
  for (auto const &obj : *objects_) {
    if (!obj->is_active()) {
      continue;
    }

    obj->draw();
  }
}

void Level::destroy_object(uint32_t id) {
  for (auto it = objects_->begin(); it != objects_->end(); ++it) {
    if (it->get()->get_id() == id) {
      objects_->erase(it);
      return;
    }
  }
}
