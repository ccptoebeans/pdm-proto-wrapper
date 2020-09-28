include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_exe test.cpp ../src/semver.cpp ../src/semver.h)
target_link_libraries(test_exe PRIVATE gtest gtest_main)
set_property(TARGET test_exe PROPERTY CXX_STANDARD 17)
