#include <engine/engine.hpp>

#include <constants.hpp>
#include <engine/input.hpp>
#include <engine/time.hpp>
#include <level/level.hpp>
#include <network/client.hpp>
#include <network/network.hpp>
#include <network/server.hpp>
#include <objects/object.hpp>
#include <objects/player.hpp>
#include <objects/spawner.hpp>

#include <cstdlib>

#include <ncurses.h>

Engine::Engine() {
  std::srand(static_cast<int>(Time::time()));

  init_screen();
  init_level();
  init_spawner();
}

Engine::~Engine() { endwin(); }

void Engine::init_screen() {
  initscr();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  start_color();

  init_pair(COLOR_PLAYER_FIRST, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_PLAYER_SECOND, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_PLAYER_THIRD, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_PLAYER_FOURTH, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_APPLE, COLOR_GREEN, COLOR_BLACK);
}

void Engine::init_level() {
  level_ = std::make_unique<Level>();

  for (int i = 0; i < LEVEL_SIZE_X; i += 2) {
    auto obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(i, 0);
    obj->set_layer(LAYER_DEFUALT);

    obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(i, LEVEL_SIZE_Y);
    obj->set_layer(LAYER_DEFUALT);
  }

  for (int i = 1; i < LEVEL_SIZE_Y; ++i) {
    auto obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(0, i);
    obj->set_layer(LAYER_DEFUALT);

    obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(LEVEL_SIZE_X - 2, i);
    obj->set_layer(LAYER_DEFUALT);
  }
}

void Engine::init_players() {
  const int members = static_cast<int>(network_->members());
  for (int i = 0; i < members; ++i) {
    auto player = level_->spawn_object<Player>(level_.get());
    player->set_color(COLOR_PAIR(COLOR_PLAYER_FIRST + i));
    player->set_spawn_position(4, 6 * (i + 1));
    player->set_playable(false);
    players_.push_back(player);
  }

  players_[network_->id()]->set_playable(true);
}

void Engine::init_spawner() {
  spawner_ = level_->spawn_object<Spawner>(level_.get());
  spawner_->set_position(-500, -500);
  spawner_->set_spawn_zone(0, 0, LEVEL_SIZE_Y, LEVEL_SIZE_X - 2);
}

void Engine::reset() {
  for (auto &player : players_) {
    player->reset();
    player->set_size(3);
  }

  spawner_->spawn();

  is_over_ = false;
  is_reset_ = false;
}

void Engine::input() {
  if (Input::is_pressed(Input::Key::Q)) {
    is_stopped_ = true;
  } else if (Input::is_pressed(Input::Key::R)) {
    if (is_over_) {
      is_reset_ = true;
    }

    if (is_paused_) {
      is_paused_ = false;
    }
  }
}

void Engine::update_over() {
  std::size_t alive{0};
  for (auto &player : players_) {
    if (player->is_alive()) {
      ++alive;
    }
  }

  if (alive <= 1) {
    is_over_ = true;
  }
}

void Engine::update_bounds() {
  const std::size_t id = network_->id();
  if (id >= players_.size()) {
    return;
  }

  auto &player = players_[id];
  if (player->get_position_x() == 0) {
    player->set_position(LEVEL_SIZE_X - 5, player->get_position_y());
    player->move_x(1);
  } else if (player->get_position_x() == LEVEL_SIZE_X - 2) {
    player->set_position(3, player->get_position_y());
    player->move_x(-1);
  } else if (player->get_position_y() == 0) {
    player->set_position(player->get_position_x(), LEVEL_SIZE_Y - 2);
    player->move_y(1);
  } else if (player->get_position_y() == LEVEL_SIZE_Y) {
    player->set_position(player->get_position_x(), 2);
    player->move_y(-1);
  }
}

void Engine::update_accept() {
  network_->accept();

  network_->add_field(is_paused_);
  network_->send();

  if (!is_paused_) {
    init_players();
    reset();
    update_network_ = [this]() { update_server(); };
  }
}

void Engine::update_connect() {
  is_recv_success_ = network_->recv();
  is_paused_ = network_->next_field();

  if (!is_paused_) {
    init_players();
    reset();
    update_network_ = [this]() { update_client(); };
  }
}

void Engine::update_server() {
  is_recv_success_ = network_->recv();
  if (!is_recv_success_) {
    return;
  }

  for (std::size_t i = 0; i < players_.size() - 1; ++i) {
    const int id = network_->next_field();
    players_[id]->set_alive(network_->next_field());
    players_[id]->set_sprite(static_cast<char>(network_->next_field()));

    const std::size_t previous_size = players_[id]->get_size();
    players_[id]->set_size(network_->next_field());
    if (previous_size != players_[id]->get_size()) {
      spawner_->spawn();
    }

    for (std::size_t j = 0; j < players_[id]->get_size(); ++j) {
      players_[id]->parts_[j]->set_position_x(network_->next_field());
      players_[id]->parts_[j]->set_position_y(network_->next_field());
    }
  }

  network_->add_field(is_over_);
  network_->add_field(is_reset_);

  if (is_reset_) {
    reset();
  }

  for (auto &player : players_) {
    network_->add_field(player->is_alive());
    network_->add_field(player->get_sprite());
    network_->add_field(player->get_size());
    for (std::size_t i = 0; i < player->get_size(); ++i) {
      network_->add_field(player->parts_[i]->get_position_x());
      network_->add_field(player->parts_[i]->get_position_y());
    }
  }

  network_->add_field(spawner_->apple_->get_position_x());
  network_->add_field(spawner_->apple_->get_position_y());

  network_->send();
}

void Engine::update_client() {
  const std::size_t id = network_->id();
  network_->add_field(id);
  network_->add_field(players_[id]->is_alive());
  network_->add_field(players_[id]->get_sprite());
  network_->add_field(players_[id]->get_size());
  for (std::size_t i = 0; i < players_[id]->get_size(); ++i) {
    network_->add_field(players_[id]->parts_[i]->get_position_x());
    network_->add_field(players_[id]->parts_[i]->get_position_y());
  }

  network_->send();

  is_recv_success_ = network_->recv();
  if (!is_recv_success_) {
    return;
  }

  is_over_ = network_->next_field();
  is_reset_ = network_->next_field();

  if (is_reset_) {
    reset();
  }

  for (auto &player : players_) {
    player->set_alive(network_->next_field());
    player->set_sprite(static_cast<char>(network_->next_field()));
    player->set_size(network_->next_field());
    for (std::size_t i = 0; i < player->get_size(); ++i) {
      player->parts_[i]->set_position_x(network_->next_field());
      player->parts_[i]->set_position_y(network_->next_field());
    }
  }

  spawner_->apple_->set_position_x(network_->next_field());
  spawner_->apple_->set_position_y(network_->next_field());
  spawner_->apple_->set_active(true);
}

void Engine::update() {
  input();

  if (is_over_) {
    return;
  }

  if (is_paused_) {
    return;
  }

  level_->update();

  update_bounds();
  update_over();
}

void Engine::draw_ui() const {
  std::string msg = "PLAYERS " + std::to_string(network_->members());
  mvaddstr(0, 1, msg.c_str());

  mvaddstr(0, LEVEL_SIZE_X + 1, "<SCORES>");
  for (std::size_t i = 0; i < players_.size(); ++i) {
    msg = "PLAYER " + std::to_string(i + 1) + ": " +
        std::to_string((players_[i]->get_size() - 3) * 10);
    attron(COLOR_PAIR(COLOR_PLAYER_FIRST + i));
    mvaddstr(2 + i, LEVEL_SIZE_X + 1, msg.c_str());
    attroff(COLOR_PAIR(COLOR_PLAYER_FIRST + i));

    if (!players_[i]->is_alive()) {
      mvaddstr(2 + i, LEVEL_SIZE_X + 11, "WASTED");
    }
  }

  mvaddstr(LEVEL_SIZE_Y, LEVEL_SIZE_X + 1, "W/A/S/D - SNAKE CONTROL");
  mvaddstr(LEVEL_SIZE_Y - 1, LEVEL_SIZE_X + 1, "Q       - QUIT GAME");
  if (network_->id() == 0) {
    mvaddstr(LEVEL_SIZE_Y - 2, LEVEL_SIZE_X + 1, "R       - (RE)START GAME");
  }
}

void Engine::draw() const {
  clear();

  level_->draw();
  draw_ui();
}

void Engine::main_loop() {
  Time::dt_ = 1.0 / 60;

  const double ms_per_update = Time::dt();

  double current_update_time = 0;
  double previous_update_time = Time::time();

  const std::size_t max_draw_skips = 4;
  std::size_t current_draw_skips = 0;

  double frame_lag = 0;

  const double tick_rate_ = 1.0 / 60;
  double last_net_update = 0;

  while (!is_stopped_) {
    current_update_time = Time::time();
    Time::dt_ = current_update_time - previous_update_time;
    previous_update_time = current_update_time;

    frame_lag += Time::dt();
    while (frame_lag > ms_per_update && current_draw_skips < max_draw_skips) {
      Input::input();
      update();
      Input::update();

      if (Time::time() - last_net_update > tick_rate_) {
        update_network_();
        if (!is_recv_success_) {
          is_stopped_ = true;
          break;
        }

        last_net_update = Time::time();
      }

      frame_lag -= ms_per_update;
      current_draw_skips++;
    }

    if (frame_lag < 0) {
      frame_lag = 0;
    }

    current_draw_skips = 0;

    draw();
  }
}

void Engine::run_server() {
  network_ = std::make_unique<Server>();
  update_network_ = [this]() { update_accept(); };

  main_loop();
}

void Engine::run_client(std::string_view ip) {
  network_ = std::make_unique<Client>();
  network_->connect(ip);
  update_network_ = [this]() { update_connect(); };

  spawner_->set_active(false);

  main_loop();
}
