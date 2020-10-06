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

add_library(platform_pdm_proto_wrapper STATIC
    generated/eve_launcher/pdm.pb.cc
    include/pdm/protobuf.h
    src/protobuf.cpp
    src/semver.h
    src/semver.cpp
)
target_link_libraries(platform_pdm_proto_wrapper PUBLIC pdm protobuf::libprotobuf)
target_include_directories(platform_pdm_proto_wrapper PUBLIC generated)

option(USE_WITH_PROTOBUF_DLL "Is this library going to be used with protobuf as DLL?" OFF)

if(USE_WITH_PROTOBUF_DLL)
    target_compile_definitions(platform_pdm_proto_wrapper PUBLIC PROTOBUF_USE_DLLS=1)
endif()

if (APPLE)
    target_compile_definitions(platform_pdm_proto_wrapper PRIVATE "DllExport=__attribute__((visibility(\"default\")))")
elseif (MSVC)
    target_compile_definitions(platform_pdm_proto_wrapper PRIVATE "DllExport=__declspec(dllexport)")
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