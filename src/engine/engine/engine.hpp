#pragma once

#include <memory>
#include <vector>

class Level;
class Player;

class Engine {
public:
  Engine();
  ~Engine();

  void run();

private:
  void init_screen();
  void init_level();
  void init_players();

  void input();
  void update();
  void update_net();
  void draw() const;

private:
  bool is_stopped_{false};
  bool is_over_{false};

  std::unique_ptr<Level> level_{};

  std::vector<std::shared_ptr<Player>> players_;
};
