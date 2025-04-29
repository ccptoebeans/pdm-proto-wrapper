cmake_minimum_required(VERSION 3.16)
project(platform_pdm_proto_wrapper VERSION 2.0.0)

find_package(carbon-pdm CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

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
get_target_property(PDM_INTERFACE_INCLUDE_DIRECTORIES pdm INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(platform_pdm_proto_wrapper
        PUBLIC
            $<INSTALL_INTERFACE:include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/generated>
)

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

# Install rules
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    install(
            TARGETS platform_pdm_proto_wrapper
            EXPORT platform_pdm_proto_wrapperTargets
            CONFIGURATIONS Release
            ARCHIVE DESTINATION lib/
            LIBRARY DESTINATION lib/
    )
    # Install rule for available public headers
    install(DIRECTORY include/ DESTINATION include)

    install(
            EXPORT platform_pdm_proto_wrapperTargets
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/carbon-pdmprotowrapper
            FILE carbon-pdmprotowrapper.cmake
    )

    install(
            FILES carbon-pdmprotowrapperConfig.cmake
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/carbon-pdmprotowrapper
    )
endif()