#include <engine/engine.hpp>

#include <constants.hpp>
#include <engine/input.hpp>
#include <engine/time.hpp>
#include <level/level.hpp>
#include <objects/object.hpp>

#include <cstdlib>

#include <ncurses.h>

Engine::Engine() {
  std::srand(static_cast<int>(Time::time()));

  init_screen();
  init_level();
}

Engine::~Engine() { endwin(); }

void Engine::init_screen() {
  initscr();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  start_color();

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
}

void Engine::init_level() {
  level_ = std::make_unique<Level>();

  for (int i = 0; i < LEVEL_SIZE_X; i += 2) {
    auto obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(i, 0);
    obj->set_layer(1);

    obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(i, LEVEL_SIZE_Y);
    obj->set_layer(1);
  }

  for (int i = 1; i < LEVEL_SIZE_Y; ++i) {
    auto obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(0, i);
    obj->set_layer(1);

    obj = level_->spawn_object<Object>();
    obj->set_sprite('*');
    obj->set_position(LEVEL_SIZE_X - 2, i);
    obj->set_layer(1);
  }
}

void Engine::input() {
  if (Input::is_pressed(Input::Key::Q)) {
    is_stopped_ = true;
  }
}

void Engine::update() {
  input();

  if (is_over_) {
    return;
  }

  level_->update();
}

void Engine::update_net() {}

void Engine::draw() const { level_->draw(); }

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
