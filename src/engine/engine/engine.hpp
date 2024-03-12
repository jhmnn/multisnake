#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

class Level;
class Player;
class Spawner;
class Network;

class Engine {
public:
  Engine();
  ~Engine();

  void run_server();
  void run_client(std::string_view ip);

private:
  void init_screen();
  void init_level();
  void init_players();
  void init_spawner();

  void reset();

  void input();

  void update();
  void update_over();
  void update_bounds();
  void update_accept();
  void update_connect();
  void update_server();
  void update_client();

  void draw() const;
  void draw_ui() const;

  void main_loop();

private:
  bool is_stopped_{false};
  bool is_over_{false};
  bool is_paused_{true};
  bool is_reset_{false};

  std::unique_ptr<Level> level_{};

  std::vector<std::shared_ptr<Player>> players_{};
  std::shared_ptr<Spawner> spawner_{};

  std::unique_ptr<Network> network_;
  bool is_recv_success_{true};

  std::function<void()> update_network_{};
};
