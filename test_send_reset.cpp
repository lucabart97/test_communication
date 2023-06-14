#include "proto/command.pb.h"
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define IPADDRESS "127.0.0.1"
#define UDP_PORT 12345

using boost::asio::ip::address;
using boost::asio::ip::udp;

using namespace std;
int main(int argc, char const *argv[]) {

  tk::Command command;

  boost::asio::io_service io_service;
  udp::socket socket(io_service);
  udp::endpoint endpoint =
      udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);
  socket.open(udp::v4());
  boost::system::error_code err;

  std::string buffer0 = {0x01, 0x01, 0x00};
  command.set_command(tk::Type::RESET_LOC);
  command.AppendToString(&buffer0);
  auto sent0 = socket.send_to(boost::asio::buffer(buffer0), endpoint, 0, err);

  socket.close();
  return 0;
}
