#include <engine/engine.hpp>

#include <constants.hpp>
#include <engine/input.hpp>
#include <engine/time.hpp>
#include <level/level.hpp>
#include <objects/object.hpp>
#include <objects/player.hpp>
#include <objects/spawner.hpp>

#include <cstdlib>

#include <ncurses.h>

Engine::Engine() {
  std::srand(static_cast<int>(Time::time()));

  init_screen();
  init_level();
  init_players();
  init_spawner();

  reset();
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
  for (int i = 0; i < 1; i++) {
    auto player = level_->spawn_object<Player>(level_.get());
    player->set_color(COLOR_PAIR(COLOR_PLAYER_FIRST + i));
    player->set_spawn_position(4, 3 * (i + 1));
    player->set_playable(false);
    players_.push_back(player);
  }

  players_[0]->set_playable(true);
}

void Engine::init_spawner() {
  spawner_ = level_->spawn_object<Spawner>(level_.get());
  spawner_->set_position(-500, -500);
  spawner_->set_spawn_zone(0, 0, LEVEL_SIZE_Y, LEVEL_SIZE_X - 2);
}

void Engine::reset() {
  for (auto &player : players_) {
    player->reset();
  }

  spawner_->spawn();

  is_over_ = false;
}

void Engine::input() {
  if (Input::is_pressed(Input::Key::Q)) {
    is_stopped_ = true;
  } else if (Input::is_pressed(Input::Key::R)) {
    if (is_over_) {
      reset();
    }
  }
}

void Engine::update_net() {}

void Engine::update_over() {
  std::size_t alive{0};
  for (auto &player : players_) {
    if (player->is_alive()) {
      ++alive;
    }
  }

  if (alive == 0) {
    is_over_ = true;
  }
}

void Engine::update_bounds() {
  for (auto &player : players_) {
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
}

void Engine::update() {
  input();

  if (is_over_) {
    return;
  }

  level_->update();

  update_bounds();
  update_over();
}

void Engine::draw() const {
  clear();

  level_->draw();
}

void Engine::run() {
  Time::dt_ = 1.0 / 60;

  const double ms_per_update = Time::dt();

  double current_update_time = 0;
  double previous_update_time = Time::time();

  const std::size_t max_draw_skips = 4;
  std::size_t current_draw_skips = 0;

  double frame_lag = 0;

  const double tick_rate_ = 1.0 / 30;
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
        update_net();
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
