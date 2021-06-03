cmake_minimum_required(VERSION 3.16)
project(platform_pdm_proto_wrapper)

add_subdirectory(platform-pdm)

include(FetchContent)

FetchContent_Populate(
  protobuf
  SOURCE_DIR protobuf
  URL https://github.com/protocolbuffers/protobuf/releases/download/v3.6.0/protobuf-cpp-3.6.0.tar.gz
)
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/protobuf/cmake)
find_package(protobuf REQUIRED)

option(USE_EVE_PUBLIC_DOMAIN "ON for using the eve_public and OFF for using the eve_launcher proto domain" OFF)

if(USE_EVE_PUBLIC_DOMAIN)
    set(PROTO_CC_FILE generated/eve_public/app/platform.pb.cc)
else()
    set(PROTO_CC_FILE generated/eve_launcher/pdm.pb.cc)
endif()

add_library(platform_pdm_proto_wrapper STATIC
    ${PROTO_CC_FILE}
    include/pdm/protobuf.h
    src/protobuf.cpp
    src/semver.h
    src/semver.cpp
)
target_link_libraries(platform_pdm_proto_wrapper PUBLIC pdm protobuf::libprotobuf)
target_include_directories(platform_pdm_proto_wrapper PUBLIC generated)

if(USE_EVE_PUBLIC_DOMAIN)
    target_compile_definitions(platform_pdm_proto_wrapper PUBLIC USE_EVE_PUBLIC_DOMAIN)
endif()

if(BUILD_SHARED_LIBS)
    target_compile_definitions(platform_pdm_proto_wrapper PUBLIC PROTOBUF_USE_DLLS=1)
endif()

set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_STANDARD 17)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY CXX_VISIBILITY_PRESET hidden)
set_property(TARGET platform_pdm_proto_wrapper PROPERTY VISIBILITY_INLINES_HIDDEN YES)

option(ENABLE_TESTS "Build with tests" ON)

if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()