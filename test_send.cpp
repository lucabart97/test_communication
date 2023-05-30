#include "proto/grid.pb.h"
#include "proto/position.pb.h"
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#define IPADDRESS "127.0.0.1"
#define UDP_PORT 12345
#define ITER 10

using boost::asio::ip::address;
using boost::asio::ip::udp;

using namespace std;
int main(int argc, char const *argv[]) {

  // set-up grid
  tk::Grid grid;
  grid.set_resolution(0.1F);
  grid.set_size(4);

  for (int i = 0; i < 4*4; i++) {
    grid.add_data(i);
  }

  // set-up pose
  tk::Position pose;
  pose.set_gps_latitude(1000.0);
  pose.set_gps_longitude(2000.0);

  boost::asio::io_service io_service;
  udp::socket socket(io_service);
  udp::endpoint endpoint =
      udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);
  socket.open(udp::v4());
  boost::system::error_code err;

  for (size_t i = 0; i < ITER; i++) {

    // send occupancy grid
    std::string buffer0 = {0x03, 0x01, 0x00};
    grid.AppendToString(&buffer0);
    auto sent0 = socket.send_to(boost::asio::buffer(buffer0), endpoint, 0, err);

    // send pose
    std::string buffer1 = {0x05, 0x01, 0x00};
    pose.AppendToString(&buffer1);
    auto sent1 = socket.send_to(boost::asio::buffer(buffer1), endpoint, 0, err);

    std::cout << "send counter: " << i << "\n";
    sleep(1);
  }

  socket.close();
  return 0;
}
