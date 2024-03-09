#pragma once

#include <chrono>

class Engine;

class Time {
  friend class Engine;

public:
  static double dt();
  static double time();

private:
  static double dt_;
};
