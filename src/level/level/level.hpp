#pragma once

#include <objects/object.hpp>

#include <memory>
#include <vector>

using Objects = std::vector<std::shared_ptr<Object>>;

class Level {
public:
  Level();

  template <class T, typename... Args>
  std::shared_ptr<T> spawn_object(Args &&...args) {
    static_assert(
        std::is_base_of<Object, T>::value,
        "T must be a derived of Object class");

    std::shared_ptr<T> obj = std::make_shared<T>(args...);
    objects_->push_back(obj);

    return obj;
  }

  template <class T> std::shared_ptr<T> get_object(uint32_t id) {
    static_assert(
        std::is_base_of<Object, T>::value,
        "T must be a derived of Object class");

    for (auto const &obj : *objects_) {
      if (obj->get_id() == id) {
        std::shared_ptr<T> target = std::dynamic_pointer_cast<T>(obj);
        return target;
      }
    }

    return nullptr;
  }

  void destroy_object(uint32_t id);

  void update();
  void draw() const;

public:
  std::shared_ptr<Objects> objects_;
};
