#pragma once

#include <chrono>

class Engine;

class Time {
  friend class Engine;

private:
  static double dt_;

public:
  static double dt() { return dt_; }
  static double time() {
    return std::chrono::duration<double>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
  }
};
