cmake_minimum_required(VERSION 3.16)
project(platform_pdm_proto_wrapper VERSION 2.0.0)

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
    generated/eve_public/app/platform.pb.cc
    generated/eve_launcher/pdm.pb.cc
    include/pdm/protobuf.h
    src/protobuf_template.h
    src/protobuf_public.cpp
    src/protobuf_launcher.cpp
    src/semver.h
    src/semver.cpp
)
target_link_libraries(platform_pdm_proto_wrapper PUBLIC pdm protobuf::libprotobuf)
target_include_directories(platform_pdm_proto_wrapper PUBLIC generated)

option(USE_WITH_PROTOBUF_DLL "Is this library going to be used with protobuf as DLL?" OFF)

if(USE_WITH_PROTOBUF_DLL)
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

#Combine static libs into one lib if protobuf is static
if(NOT protobuf_BUILD_SHARED_LIBS)
    
    set(PDM_WITH_PROTO_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}pdm_with_proto${CMAKE_STATIC_LIBRARY_SUFFIX}")

    if (WIN32)
        #requires lib.exe to be available msvc
        add_custom_command(TARGET platform_pdm_proto_wrapper POST_BUILD
        COMMAND lib.exe /OUT:$<TARGET_FILE_DIR:platform_pdm_proto_wrapper>/${PDM_WITH_PROTO_NAME}
                $<TARGET_FILE:platform_pdm_proto_wrapper>
                $<TARGET_FILE:pdm>
                $<TARGET_FILE:protobuf::libprotobuf>
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    elseif (APPLE)
        add_custom_command(TARGET platform_pdm_proto_wrapper POST_BUILD
        COMMAND libtool -static -o $<TARGET_FILE_DIR:platform_pdm_proto_wrapper>/${PDM_WITH_PROTO_NAME}
                $<TARGET_FILE:platform_pdm_proto_wrapper>
                $<TARGET_FILE:pdm>
                $<TARGET_FILE:protobuf::libprotobuf>
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    else()
        message(FATAL_ERROR "Post build step to combine libs doesn't support current platform")
    endif()
endif()