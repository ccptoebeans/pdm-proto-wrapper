cmake_minimum_required(VERSION 3.16)
project(platform_pdm_proto_wrapper)

include(FetchContent)

FetchContent_Populate(
  protobuf
  SOURCE_DIR protobuf
  URL https://github.com/protocolbuffers/protobuf/releases/download/v3.13.0/protobuf-cpp-3.13.0.tar.gz
)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake)
find_package(protobuf REQUIRED)

FetchContent_Declare(
  pdm
  GIT_REPOSITORY git@github.com:ccpgames/platform-pdm.git
  GIT_TAG        395ecaf6b7abddc7391f0282c1c1a50b777ae9c6
)
FetchContent_MakeAvailable(pdm)

add_library(platform_pdm_proto_wrapper SHARED src/protobuf.cpp include/pdm/protobuf.h generated/pdm.pb.cc)
target_link_libraries(platform_pdm_proto_wrapper PUBLIC pdm protobuf::libprotobuf)

set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_STANDARD 17)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY VISIBILITY_INLINES_HIDDEN YES)