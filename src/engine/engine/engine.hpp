#pragma once

#include <memory>

class Engine {
public:
  Engine();
  ~Engine();

  void run();

private:
  bool is_stopped_{false};
  bool is_over_{false};

private:
  void init_screen();

  void input();
  void update();
  void update_net();
  void draw() const;
};
