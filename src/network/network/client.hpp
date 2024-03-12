#pragma once

#include <network/network.hpp>

#include <string>

struct ServerSocket {
  sockaddr_in addr{};
  socklen_t len{};
};

class Client : public Network {
public:
  Client();

  bool accept() override;
  bool connect(std::string_view ip) override;

  bool send() override;
  bool recv() override;

  std::size_t members() const override;

private:
  ServerSocket server_{};

  std::size_t members_{1};
};
