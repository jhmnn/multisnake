#pragma once

#include <engine/input.hpp>
#include <objects/object.hpp>

#include <memory>
#include <vector>

class Engine;
class Level;

class Player : public Object {
  friend class Engine;

public:
  explicit Player(Level *level);

  void set_alive(bool state);
  bool is_alive() const;

  void set_playable(bool state);
  bool is_playable() const;

  void set_spawn_position(int x, int y);

  void set_direction(int x, int y);
  void set_direction_x(int value);
  void set_direction_y(int value);
  int get_direction_x() const;
  int get_direction_y() const;

  void increase_size(std::size_t count);
  void set_size(std::size_t count);
  std::size_t get_size() const;

  void reset();

  void die();

  void update() override;

private:
  void on_collision(Object *obj) override;
  void on_spawn() override;

  void input(Input::Key key);
  void update_head();
  void update_body();
  void update_movement();

private:
  std::vector<std::shared_ptr<Object>> parts_;
  std::size_t active_parts_{0};

  Level *level_{};

  bool is_alive_{true};
  bool is_playable_{true};

  double move_interval_{};
  double move_timer_{};

  int spawn_position_x_{0};
  int spawn_position_y_{0};

  int previous_position_x_{};
  int previous_position_y_{};

  int direction_x_{1};
  int direction_y_{0};
};