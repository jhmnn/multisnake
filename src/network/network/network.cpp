#include <network/network.hpp>

#include <iostream>

#include <unistd.h>

Network::Network() {
  sd_ = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd_ < 0) {
    std::cerr << "Can't open socket\n";
    exit(EXIT_FAILURE);
  }

  send_buf_.reserve(buf_size_);
  recv_buf_.resize(buf_size_, '\0');

  getsockname(sd_, reinterpret_cast<sockaddr *>(&addr_), &len_);
}

Network::~Network() { close(sd_); }

void Network::add_field(int value) {
  const std::string field = std::to_string(value);

  if (send_buf_.size() + field.size() > buf_size_) {
    return;
  }

  send_buf_ += field;
  send_buf_ += '.';
}

void Network::add_field(char value) { add_field(static_cast<int>(value)); }

void Network::add_field(std::size_t value) {
  add_field(static_cast<int>(value));
}

int Network::next_field() {
  std::string field{};

  const std::size_t field_end = recv_buf_.find('.', current_field_);
  if (field_end == std::string::npos) {
    return -1;
  }

  field = recv_buf_.substr(current_field_, field_end - current_field_);
  current_field_ = field_end + 1;

  return static_cast<int>(std::strtol(field.c_str(), nullptr, 10));
}

std::string Network::ip() const { return inet_ntoa(addr_.sin_addr); }

uint16_t Network::port() const { return ntohs(addr_.sin_port); }

std::size_t Network::id() const { return id_; }
