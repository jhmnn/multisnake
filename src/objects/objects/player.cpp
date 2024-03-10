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
  if (is_playable_) {
    input(Input::pressed_last());
  }

  if (Time::time() - move_timer_ > move_interval_) {
    update_head();
    update_body();

    move_timer_ = Time::time();
  }
}

bool Player::is_alive() const { return is_alive_; }

void Player::set_playable(bool state) { is_playable_ = state; }

bool Player::is_playable() const { return is_playable_; }

void Player::increase_size(std::size_t count) {
  if (count == 0) {
    return;
  }

  if (parts_.size() - active_parts_ >= count) {
    const auto new_size = parts_.size() + count;
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
  if (count == 0) {
    return;
  }

  if (parts_.size() < count) {
    increase_size(parts_.size() - count);
  } else if (parts_.size() > count) {
    for (std::size_t i = parts_.size() - 1; i >= count; --i) {
      parts_[i]->set_active(false);
    }

    active_parts_ = count;
  }

  update_body();
}

void Player::reset() {
  set_size(1);

  is_alive_ = true;

  sprite_ = 'c';

  move_interval_ = 0.09;
  move_timer_ = 0.0;

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
