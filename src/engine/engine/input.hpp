#pragma once

#include <vector>

#include <cstdint>

class Engine;

class Input {
  friend class Engine;

public:
  enum class Key { W, A, S, D, Q, R, Unknown };

public:
  static bool is_pressed(Key key);
  static std::vector<Key> &pressed();
  static Key pressed_last();

private:
  static uint32_t keys_states_;
  static std::vector<Key> pressed_;
  static Key pressed_last_;

private:
  static Key keycode_to_key(int kc);

  static void input();
  static void update();
};
