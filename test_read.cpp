#include "proto/grid.pb.h"
#include "proto/position.pb.h"
#include <google/protobuf/text_format.h>
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define IPADDRESS "127.0.0.1"
#define UDP_PORT 12345
#define ITER 10

using boost::asio::ip::address;
using boost::asio::ip::udp;
boost::array<char, 1024> recv_buffer;

int main(int argc, char const *argv[]) {

  boost::asio::io_service io_service;
  udp::socket socket{io_service};
  udp::endpoint remote_endpoint;
  socket.open(udp::v4());
  socket.bind(udp::endpoint(address::from_string(IPADDRESS), UDP_PORT));

  while (true) {
    size_t len =
        socket.receive_from(boost::asio::buffer(recv_buffer), remote_endpoint);
    
    if(recv_buffer[0] == 0x03) {
      tk::Grid grid;
      grid.ParseFromArray(recv_buffer.begin() + 3, recv_buffer.size() - 3);
      std::string msg;
      google::protobuf::TextFormat::PrintToString(grid, &msg);
      std::cout << msg << "\n\n";
    }

    if (recv_buffer[0] == 0x05) {
      tk::Position pose;
      pose.ParseFromArray(recv_buffer.begin() + 3, recv_buffer.size() - 3);
      std::string msg;
      google::protobuf::TextFormat::PrintToString(pose, &msg);
      std::cout << msg << "\n\n";
    }
  }
  socket.close();
  return 0;
}
