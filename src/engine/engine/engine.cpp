#include <engine/engine.hpp>

#include <engine/input.hpp>
#include <engine/time.hpp>

#include <ncurses.h>

Engine::Engine() { init_screen(); }

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
}

void Engine::update_net() {}

void Engine::draw() const {}

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
