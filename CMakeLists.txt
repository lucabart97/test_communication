cmake_minimum_required(VERSION 3.0.0)

project(tkEstoniaTest
    VERSION 0.0.1
    DESCRIPTION "estonia project test"
    LANGUAGES CXX)

add_subdirectory(proto)
find_package(Boost 1.40.0 REQUIRED system)

add_executable(test_send test_send.cpp)
target_link_libraries(test_send PUBLIC
    message
    Boost::system
)
target_include_directories(test_send PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(test_read test_read.cpp)
target_link_libraries(test_read PUBLIC
    message
    Boost::system
)
target_include_directories(test_read PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
)
