#include <objects/object.hpp>

#include <level/collision.hpp>

#include <ncurses.h>

uint32_t free_id = 0;

Object::Object() { id_ = free_id_++; }

uint32_t Object::get_id() const { return id_; }

void Object::set_active(bool state) { is_active_ = state; }

bool Object::is_active() const { return is_active_; }

void Object::set_position(int x, int y) {
  position_x_ = x;
  position_y_ = y;
}

void Object::set_position_x(int value) { position_x_ = value; }

void Object::set_position_y(int value) { position_y_ = value; }

int Object::get_position_x() const { return position_x_; }

int Object::get_position_y() const { return position_y_; }

void Object::set_direction(int x, int y) {
  direction_x_ = x;
  direction_y_ = y;
}

void Object::set_direction_x(int value) { direction_x_ = value; }

void Object::set_direction_y(int value) { direction_y_ = value; }

int Object::get_direction_x() const { return direction_x_; }

int Object::get_direction_y() const { return direction_y_; }

void Object::set_sprite(char sprite) { sprite_ = sprite; }

char Object::get_sprite() const { return sprite_; }

void Object::set_color(uint32_t color) { color_ = color; }

uint32_t Object::get_color() const { return color_; }

void Object::set_layer(uint32_t layer) { layer_ = layer; }

uint32_t Object::get_layer() const { return layer_; }

void Object::move_x(int step) {
  const int tstep = step >= 0 ? step : step * -1;
  const int nstep = step >= 0 ? 1 : -1;
  for (int i = 0; i < tstep; i++) {
    position_x_ += nstep;
    Collision::intersects(this);
  }
}

void Object::move_y(int step) {
  const int tstep = step >= 0 ? step : step * -1;
  const int nstep = step > 0 ? 1 : (step < 0 ? -1 : 0);
  for (int i = 0; i < tstep; i++) {
    position_y_ += nstep;
    Collision::intersects(this);
  }
}

void Object::draw() const {
  mvaddch(position_y_, position_x_, sprite_ | color_);
}
