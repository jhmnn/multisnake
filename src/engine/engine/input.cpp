#include <engine/input.hpp>

#include <ncurses.h>

uint32_t Input::keys_states_{0};
std::vector<Input::Key> Input::pressed_{};
Input::Key Input::pressed_last_{Key::Unknown};

Input::Key Input::keycode_to_key(int kc) {
  switch (kc) {
  case 'w':
    return Key::W;
  case 'a':
    return Key::A;
  case 's':
    return Key::S;
  case 'd':
    return Key::D;
  case 'q':
    return Key::Q;
  case 'r':
    return Key::R;
  default:
    return Key::Unknown;
  }
}

bool Input::is_pressed(Key key) {
  return (keys_states_ >> static_cast<int>(key)) == 1;
}

std::vector<Input::Key> &Input::pressed() { return pressed_; }

Input::Key Input::pressed_last() { return pressed_last_; }

void Input::input() {
  const Key current_key_ = keycode_to_key(getch());
  keys_states_ |= (1 << static_cast<int>(current_key_));
  pressed_last_ = current_key_;
  if (current_key_ != Key::Unknown) {
    pressed_.push_back(current_key_);
  }
}

void Input::update() {
  keys_states_ = 0;
  pressed_last_ = Key::Unknown;
}
