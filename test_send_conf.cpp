#include "proto/config.pb.h"
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define IPADDRESS "192.168.69.43"
#define UDP_PORT 12345

using boost::asio::ip::address;
using boost::asio::ip::udp;

using namespace std;
int main(int argc, char const *argv[]) {

  tk::Config config;

  boost::asio::io_service io_service;
  udp::socket socket(io_service);
  udp::endpoint endpoint =
      udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);
  socket.open(udp::v4());
  boost::system::error_code err;

  std::string buffer0 = {0x02, 0x01, 0x00};
  config.set_grid_resolution(1);
  config.set_grid_dim(10);
  config.AppendToString(&buffer0);
  auto sent0 = socket.send_to(boost::asio::buffer(buffer0), endpoint, 0, err);

  socket.close();
  return 0;
}
