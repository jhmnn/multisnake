#include <objects/player.hpp>

#include <engine/input.hpp>
#include <engine/time.hpp>
#include <level/level.hpp>

Player::Player(Level *level) { level_ = level; }

// void Player::on_collision(Object &obj) { }

void Player::input(Input::Key key) {
  previous_direction_x_ = direction_x_;
  previous_direction_y_ = direction_y_;

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
  move_x(direction_x_ * 2);
  move_y(direction_y_);
}

void Player::update_body() {
  int new_position_x{};
  int new_position_y{};
  int current_direction_x{};
  int current_direction_y{};

  for (std::size_t i = 1; i < parts_.size(); ++i) {
    auto &part = parts_[i - 1];

    new_position_x = part->get_position_x() - part->get_direction_x() * 2;
    new_position_y = part->get_position_y() - part->get_direction_y();
    current_direction_x = parts_[i]->get_direction_x();
    current_direction_y = parts_[i]->get_direction_y();

    parts_[i]->set_position(new_position_x, new_position_y);
    parts_[i]->set_direction(previous_direction_x_, previous_direction_y_);
    previous_direction_x_ = current_direction_x;
    previous_direction_y_ = current_direction_y;
  }
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
      // parts_.back()->set_layer(0);
      parts_.back()->set_sprite('o');
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
}

void Player::reset() {
  set_size(1);
  is_alive_ = true;
  sprite_ = 'c';
  direction_x_ = 1;
  direction_y_ = 0;
}

void Player::die() { is_alive_ = false; }

void Player::update() { update_movement(); }