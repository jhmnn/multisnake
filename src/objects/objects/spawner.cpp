#include <objects/spawner.hpp>

#include <constants.hpp>
#include <level/collision.hpp>
#include <level/level.hpp>
#include <objects/object.hpp>

#include <cstdlib>

#include <ncurses.h>

Spawner::Spawner(Level *level) {
  apple_ = level->spawn_object<Object>();
  apple_->set_layer(LAYER_APPLE);
  apple_->set_sprite(APPLE_SPRITE);
  apple_->set_color(COLOR_PAIR(COLOR_APPLE));
  apple_->set_active(false);
}

void Spawner::spawn() {
  apple_->set_active(true);

  int x = apple_->get_position_x();
  int y = apple_->get_position_y();

  while (x == apple_->get_position_x() && y == apple_->get_position_y()) {
    x = rand() % (right_bound_ - left_bound_ - 1) + left_bound_ + 2;
    y = rand() % (bottom_bound_ - up_bound_ - 1) + up_bound_ + 2;
  }

  for (int i = x % 2 == 0 ? x : x - 1; i < right_bound_; i += 2) {
    for (int j = y; j < bottom_bound_; ++j) {
      apple_->set_position(i, j);
      if (Collision::intersects(apple_.get()) == nullptr) {
        return;
      }
    }
  }

  for (int i = x % 2 == 0 ? x : x - 1; i > left_bound_; i -= 2) {
    for (int j = y; j > up_bound_; --j) {
      apple_->set_position(i, j);
      if (Collision::intersects(apple_.get()) == nullptr) {
        return;
      }
    }
  }
}

void Spawner::set_spawn_zone(
    int up_bound, int left_bound, int bottom_bound, int right_bound) {
  up_bound_ = up_bound;
  left_bound_ = left_bound;
  bottom_bound_ = bottom_bound;
  right_bound_ = right_bound;
}

void Spawner::update() {
  if (!apple_->is_active()) {
    spawn();
  }
}
