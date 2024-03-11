#include <network/server.hpp>

#include <cstring>
#include <iostream>

#include <unistd.h>

Server::Server() {
  len_ = sizeof(addr_);

  std::memset(&addr_, 0, len_);

  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  addr_.sin_port = 48683;

  if (bind(sd_, reinterpret_cast<const sockaddr *>(&addr_), len_) != 0) {
    std::cerr << "Can't bind socket to address\n";
    close(sd_);
    exit(EXIT_FAILURE);
  }

  entrance_.len = sizeof(entrance_.addr);

  recv_buf_.resize(buf_size_ * max_clients_);
}

bool Server::accept() {
  if (clients_.size() >= max_clients_) {
    return false;
  }

  if (recvfrom(
          sd_, recv_buf_.data(), buf_size_, MSG_DONTWAIT,
          reinterpret_cast<sockaddr *>(&entrance_.addr), &entrance_.len) > 0) {

    clients_.push_back(entrance_);

    add_field(members());
    add_field(clients_.size());
    sendto(
        sd_, send_buf_.data(), buf_size_, 0,
        reinterpret_cast<const sockaddr *>(&entrance_.addr), entrance_.len);
    send_buf_.clear();

    return true;
  }

  return false;
}

bool Server::connect(std::string_view ip, std::string_view port) {
  ip = "";
  port = "";
  return false;
}

bool Server::send() {
  send_buf_.insert(0, std::to_string(members()) + '.');
  for (auto &client : clients_) {
    sendto(
        sd_, send_buf_.data(), buf_size_, 0,
        reinterpret_cast<const sockaddr *>(&client.addr), client.len);
  }

  send_buf_.clear();

  return true;
}

bool Server::recv() {
  std::size_t offset{0};

  for (auto &client : clients_) {
    std::memset(recv_buf_.data(), '\0', buf_size_ * max_clients_);
    recvfrom(
        sd_, recv_buf_.data() + offset, buf_size_, 0,
        reinterpret_cast<sockaddr *>(&client.addr), &client.len);

    offset += buf_size_;
  }

  current_field_ = 0;

  return true;
}

std::size_t Server::members() const { return clients_.size() + 1; }
