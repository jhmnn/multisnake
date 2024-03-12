#include <engine/engine.hpp>

#include <iostream>

void print_usage(std::string_view prog) {
  std::cout << "Create a game: " << prog << '\n'
            << "Join the game: " << prog << " <IP>\n";
}

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 2) {
    print_usage(argv[0]);
    return 0;
  }

  Engine engine;

  if (argc == 1) {
    engine.run_server();
  } else {
    engine.run_client(argv[1]);
  }

  return 0;
}
