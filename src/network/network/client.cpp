#include <network/client.hpp>

#include <cstring>
#include <iostream>

Client::Client() {
  timeval time{};
  time.tv_sec = 1;
  time.tv_usec = 0;

  setsockopt(
      sd_, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&time),
      sizeof(time));
}

bool Client::accept() { return false; }

bool Client::connect(std::string_view ip) {
  server_.len = sizeof(server_.addr);

  std::memset(&server_.addr, 0, server_.len);

  server_.addr.sin_family = AF_INET;

  server_.addr.sin_port = 48683;
  if (inet_pton(AF_INET, ip.data(), &server_.addr.sin_addr) != 1) {
    std::cerr << "Invalid server IP\n";
    exit(EXIT_FAILURE);
  }

  add_field(0);
  send();

  recv();
  id_ = next_field();

  return true;
}

bool Client::send() {
  if (sendto(
          sd_, send_buf_.data(), buf_size_, 0,
          reinterpret_cast<const sockaddr *>(&server_.addr), server_.len) < 0) {
    send_buf_.clear();
    return false;
  }

  send_buf_.clear();
  return true;
}

bool Client::recv() {
  std::memset(recv_buf_.data(), '\0', buf_size_);
  if (recvfrom(
          sd_, recv_buf_.data(), buf_size_, 0,
          reinterpret_cast<sockaddr *>(&server_.addr), &server_.len) < 0) {
    return false;
  }

  current_field_ = 0;

  members_ = next_field();

  return true;
}

std::size_t Client::members() const { return members_; }
