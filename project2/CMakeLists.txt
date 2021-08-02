cmake_minimum_required(VERSION 3.10.2) 
project(DistributedSnapshot) 

INCLUDE_DIRECTORIES(include)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread")

add_executable(Main
    src/main.cpp
    src/util/config_parser.cpp
    src/util/string_helper.cpp
    src/util/test_interval.cpp
    src/core/clock.cpp
    src/core/server.cpp
    src/core/client.cpp
    src/core/client_manager.cpp
    src/core/l_mutex.cpp
    src/core/request_queue.cpp
)
