cmake_minimum_required(VERSION 3.21)
project(cp_template)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(.)
include_directories(../)

enable_testing()
include(GoogleTest)

add_executable(ModularTest modular_test.cc)
target_link_libraries(ModularTest gtest_main)
gtest_discover_tests(ModularTest)

add_executable(DsuTest dsu_test.cc)
target_link_libraries(DsuTest gtest_main)
gtest_discover_tests(DsuTest)