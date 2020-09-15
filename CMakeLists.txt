cmake_minimum_required(VERSION 3.16)
project(platform_pdm_proto_wrapper)

set(CMAKE_CXX_STANDARD 17)

find_package(protobuf REQUIRED)

add_library(platform_pdm_proto_wrapper SHARED src/protobuf.cpp include/pdm/protobuf.h)