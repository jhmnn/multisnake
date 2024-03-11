#include <objects/player.hpp>

#include <constants.hpp>
#include <engine/input.hpp>
#include <engine/time.hpp>
#include <level/level.hpp>

Player::Player(Level *level) {
  level_ = level;

  layer_ = LAYER_PLAYER;
}

void Player::on_collision(Object *obj) {
  if (obj->get_layer() == LAYER_APPLE) {
    increase_size(1);
    obj->set_active(false);
  } else if (obj->get_layer() == LAYER_PLAYER) {
    die();
  }
}

void Player::on_spawn() {
  parts_.push_back(level_->get_object<Object>(id_));
  reset();
}

void Player::input(Input::Key key) {
  if (key == Input::Key::W && direction_y_ == 0) {
    direction_x_ = 0;
    direction_y_ = -1;
    move_timer_ = 0;
  } else if (key == Input::Key::A && direction_x_ == 0) {
    direction_x_ = -1;
    direction_y_ = 0;
    move_timer_ = 0;
  } else if (key == Input::Key::S && direction_y_ == 0) {
    direction_x_ = 0;
    direction_y_ = 1;
    move_timer_ = 0;
  } else if (key == Input::Key::D && direction_x_ == 0) {
    direction_x_ = 1;
    direction_y_ = 0;
    move_timer_ = 0;
  }
}

void Player::update_head() {
  previous_position_x_ = position_x_;
  previous_position_y_ = position_y_;

  if (direction_x_ != 0) {
    move_x(direction_x_ * 2);
    sprite_ = PLAYER_HEAD_SPRITE_X;
  } else {
    move_y(direction_y_);
    sprite_ = PLAYER_HEAD_SPRITE_Y;
  }
}

void Player::update_body() {
  int new_position_x{};
  int new_position_y{};

  for (std::size_t i = 1; i < active_parts_; ++i) {
    new_position_x = previous_position_x_;
    new_position_y = previous_position_y_;
    previous_position_x_ = parts_[i]->get_position_x();
    previous_position_y_ = parts_[i]->get_position_y();

    parts_[i]->set_position(new_position_x, new_position_y);
  }

  previous_position_x_ = position_x_;
  previous_position_y_ = position_y_;
}

void Player::update_movement() {
  if (!is_playable_) {
    return;
  }

  input(Input::pressed_last());

  if (Time::time() - move_timer_ > move_interval_) {
    update_head();
    update_body();

    move_timer_ = Time::time();
  }
}

void Player::set_alive(bool state) { is_alive_ = state; }

bool Player::is_alive() const { return is_alive_; }

void Player::set_playable(bool state) { is_playable_ = state; }

bool Player::is_playable() const { return is_playable_; }

void Player::set_spawn_position(int x, int y) {
  spawn_position_x_ = x;
  spawn_position_y_ = y;
}

void Player::set_direction(int x, int y) {
  direction_x_ = x;
  direction_y_ = y;
}

void Player::set_direction_x(int value) { direction_x_ = value; }

void Player::set_direction_y(int value) { direction_y_ = value; }

int Player::get_direction_x() const { return direction_x_; }

int Player::get_direction_y() const { return direction_y_; }

void Player::increase_size(std::size_t count) {
  if (count == 0) {
    return;
  }

  if (count <= parts_.size() - active_parts_) {
    const auto new_size = active_parts_ + count;
    for (; active_parts_ < new_size; ++active_parts_) {
      parts_[active_parts_]->set_active(true);
    }
  } else {
    for (std::size_t i = 0; i < count; ++i) {
      parts_.push_back(level_->spawn_object<Object>());
      parts_.back()->set_layer(LAYER_PLAYER);
      parts_.back()->set_sprite(PLAYER_BODY_SPRITE);
      parts_.back()->set_color(color_);
    }

    active_parts_ += count;
  }

  update_body();
}

void Player::set_size(std::size_t count) {
  if (count == 0 || count == active_parts_) {
    return;
  }

  if (count > active_parts_) {
    increase_size(count - active_parts_);
  } else {
    for (; active_parts_ > count;) {
      parts_[--active_parts_]->set_active(false);
    }
  }

  update_body();
}

std::size_t Player::get_size() const { return active_parts_; }

void Player::reset() {
  set_size(1);

  is_alive_ = true;

  sprite_ = 'c';

  move_interval_ = 0.11;
  move_timer_ = 0.0;

  position_x_ = spawn_position_x_;
  position_y_ = spawn_position_y_;

  direction_x_ = 1;
  direction_y_ = 0;

  active_parts_ = 1;
}

void Player::die() { is_alive_ = false; }

void Player::update() {
  if (!is_alive_) {
    return;
  }

  update_movement();
}
