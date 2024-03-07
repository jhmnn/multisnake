#pragma once

#include <engine/input.hpp>
#include <objects/object.hpp>

#include <memory>
#include <vector>

class Level;

class Player : public Object {
public:
  explicit Player(Level *level);

  bool is_alive() const;

  void set_playable(bool state);
  bool is_playable() const;

  void increase_size(std::size_t count);
  void set_size(std::size_t count);

  void reset();

  void die();

  void update() override;

private:
  void on_collision(Object *obj) override;

  void input(Input::Key key);
  void update_head();
  void update_body();
  void update_movement();

private:
  std::vector<std::shared_ptr<Object>> parts_;
  std::size_t active_parts_{0};

  Level *level_;

  bool is_alive_{true};
  bool is_playable_{true};

  double move_interval_{};
  double move_timer_{};

  int previous_direction_x_{};
  int previous_direction_y_{};
};