cmake_minimum_required(VERSION 3.20) 
project(DistributedSnapshot) 

INCLUDE_DIRECTORIES(include)
add_executable(Main
    src/main.cpp
    src/util/config_parser.cpp
)