#pragma once

#include <objects/object.hpp>

#include <memory>

class Engine;
class Level;

class Spawner : public Object {
  friend class Engine;

public:
  explicit Spawner(Level *level);

  void spawn();

  void set_spawn_zone(
      int up_bound, int left_bound, int bottom_bound, int right_bound);

  void update() override;

private:
  std::shared_ptr<Object> apple_;

  int up_bound_{};
  int left_bound_{};
  int bottom_bound_{};
  int right_bound_{};
};
