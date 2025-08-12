find_package(GTest CONFIG REQUIRED)

add_executable(test_exe test.cpp)
target_link_libraries(test_exe PRIVATE GTest::gtest GTest::gtest_main platform_pdm_proto_wrapper)
set_property(TARGET test_exe PROPERTY CXX_STANDARD 17)

if (WIN32 AND "${CMAKE_MSVC_RUNTIME_LIBRARY}" MATCHES "MultiThreadedDLL")
    add_custom_command(TARGET test_exe POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:test_exe> $<TARGET_FILE_DIR:test_exe>
            COMMAND_EXPAND_LISTS
    )
endif ()
