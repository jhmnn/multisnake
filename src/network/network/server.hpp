#pragma once

#include <network/network.hpp>

#include <vector>

struct ClientSocket {
  sockaddr_in addr{};
  socklen_t len{};
};

class Server : public Network {
public:
  Server();

  bool accept() override;
  bool connect(std::string_view ip, std::string_view port) override;

  bool send() override;
  bool recv() override;

  std::size_t members() const override;

private:
  ClientSocket entrance_{};

  std::size_t max_clients_{4};
  std::vector<ClientSocket> clients_{};
};
