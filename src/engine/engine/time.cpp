#include <engine/time.hpp>

double Time::dt_{0};

double Time::dt() { return dt_; }

double Time::time() {
  return std::chrono::duration<double>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}
