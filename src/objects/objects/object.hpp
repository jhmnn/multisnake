#pragma once

#include <cstdint>

class Collision;

class Object {
  friend class Collision;

public:
  Object();
  virtual ~Object() = default;

  uint32_t get_id() const;

  void set_active(bool state);
  bool is_active() const;

  void set_position(int x, int y);
  void set_position_x(int value);
  void set_position_y(int value);
  int get_position_x() const;
  int get_position_y() const;

  void set_direction(int x, int y);
  void set_direction_x(int value);
  void set_direction_y(int value);
  int get_direction_x() const;
  int get_direction_y() const;

  void set_sprite(char sprite);
  char get_sprite() const;

  void set_color(uint32_t color);
  uint32_t get_color() const;

  void set_layer(uint32_t layer);
  uint32_t get_layer() const;

  void move_x(int step);
  void move_y(int step);

  virtual void update();
  void draw() const;

private:
  virtual void on_collision(Object &obj);

protected:
  uint32_t id_{};

  bool is_active_{true};

  int position_x_{0};
  int position_y_{0};

  int direction_x_{1};
  int direction_y_{0};

  char sprite_{' '};
  uint32_t color_{0};

  uint32_t layer_{0};

private:
  static uint32_t free_id_;
};
