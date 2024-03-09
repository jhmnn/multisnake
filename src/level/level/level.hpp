#pragma once

#include <memory>
#include <vector>

class Object;

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

  void destroy_object(uint32_t id);

  void update();
  void draw() const;

public:
  std::shared_ptr<Objects> objects_;
};
