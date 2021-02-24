cmake_minimum_required(VERSION 3.8)
project(tcp)



add_executable(send src/send.cpp)
add_executable(receive src/receive.cpp)

add_executable(send_mt src/send_mt.cpp)
add_executable(receive_mt src/receive_mt.cpp)



target_link_libraries(send_mt pthread)
target_link_libraries(receive_mt pthread)