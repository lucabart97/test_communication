#include "proto/grid.pb.h"
#include "proto/position.pb.h"
#include <google/protobuf/text_format.h>
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#define UDP_PORT 12345
#define ITER 10

using boost::asio::ip::address;
using boost::asio::ip::udp;
boost::array<char, 66000> recv_buffer;
std::string data;

cv::Mat vizFloat2colorMap(cv::Mat map, float aMin = 0.0f, float aMax = 0.0f);

int main(int argc, char const *argv[]) {

  boost::asio::io_service io_service;
  udp::socket socket(io_service, udp::endpoint(udp::v4(), UDP_PORT));
  udp::endpoint remote_endpoint;

  while (true) {
    size_t len =
        socket.receive_from(boost::asio::buffer(recv_buffer), remote_endpoint);

    if (recv_buffer[0] == 0x03) {

      data.append(recv_buffer.begin() + 3, len - 3);

      if (recv_buffer[1] == (recv_buffer[2] + 1)) {
        tk::Grid grid;
        grid.ParseFromString(data);

        if((grid.size()*grid.size()) != grid.data_size()){
            std::cerr<<"error, expected "<<grid.size()*grid.size()<<" got "<<grid.data_size()<<"\n";
        }

        cv::Mat lrawImg(
            cv::Size(static_cast<int>(grid.size()),
                     static_cast<int>(grid.size())),
            CV_32S,
            reinterpret_cast<void *>(grid.mutable_data()->mutable_data()));

        cv::Mat lrawFloat;
        lrawImg.convertTo(lrawFloat, CV_32F, 0.01);
        cv::Mat lvizImg = vizFloat2colorMap(lrawFloat, -2.0, 1.0);

        cv::namedWindow("lgridmap", cv::WINDOW_NORMAL);
        cv::imshow("lgridmap", lvizImg);
        cv::waitKey(1);

        data.clear();
      }
    }
  }
  socket.close();
  return 0;
}

cv::Mat vizFloat2colorMap(cv::Mat map, float aMin, float aMax) {
  double min;
  double max;
  if (aMin == 0 && aMax == 0) {
    cv::minMaxIdx(map, &min, &max);
  } else {
    min = aMin;
    max = aMax;
  }

  cv::Mat adjMap;
  // expand your range to 0..255. Similar to histEq();
  double scale = 255.0 / (max - min);
  map.convertTo(adjMap, CV_8UC1, scale, -min * scale);
  // return adjMap;

  cv::Mat falseColorsMap;
  cv::applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_PARULA);
  return falseColorsMap;
}
