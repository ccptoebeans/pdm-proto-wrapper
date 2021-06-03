add_executable(test_exe test.cpp ../src/semver.cpp ../src/semver.h)
target_link_libraries(test_exe PRIVATE gtest gtest_main platform_pdm_proto_wrapper)
set_property(TARGET test_exe PROPERTY CXX_STANDARD 17)
