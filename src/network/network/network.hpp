#pragma once

#include <string>
#include <vector>

#include <arpa/inet.h>

class Network {
public:
  Network();
  virtual ~Network();

  virtual bool accept() = 0;
  virtual bool connect(std::string_view ip) = 0;

  void add_field(int value);
  void add_field(char value);
  void add_field(std::size_t value);
  int next_field();

  virtual bool send() = 0;
  virtual bool recv() = 0;

  std::string ip() const;
  uint16_t port() const;

  std::size_t id() const;

  virtual std::size_t members() const = 0;

protected:
  int sd_{};
  sockaddr_in addr_{};
  socklen_t len_{};

  std::size_t id_{0};

  std::size_t buf_size_{1024};
  std::string send_buf_{};
  std::string recv_buf_{};

  std::size_t current_field_{0};
};
