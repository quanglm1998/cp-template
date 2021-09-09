cmake_minimum_required(VERSION 3.21)
project(cp_template)

set(CMAKE_CXX_STANDARD 17)

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

add_executable(DsuRollBackTest dsu_rollback_test.cc)
target_link_libraries(DsuRollBackTest gtest_main)
gtest_discover_tests(DsuRollBackTest)

add_executable(BitTest bit_test.cc)
target_link_libraries(BitTest gtest_main)
gtest_discover_tests(BitTest)

add_executable(DinicFlowTest dinic_flow_test.cc)
target_link_libraries(DinicFlowTest gtest_main)
gtest_discover_tests(DinicFlowTest)

add_executable(DinicFlowScalingTest dinic_flow_scaling_test.cc)
target_link_libraries(DinicFlowScalingTest gtest_main)
gtest_discover_tests(DinicFlowScalingTest)

add_executable(SparseTableTest sparse_table_test.cc)
target_link_libraries(SparseTableTest gtest_main)
gtest_discover_tests(SparseTableTest)

add_executable(FftTest fft_test.cc)
target_link_libraries(FftTest gtest_main)
gtest_discover_tests(FftTest)

add_executable(FftModTest fft_mod_test.cc)
target_link_libraries(FftModTest gtest_main)
gtest_discover_tests(FftModTest)

add_executable(NttTest ntt_test.cc)
target_link_libraries(NttTest gtest_main)
gtest_discover_tests(NttTest)

add_executable(NttWithISTest ntt_with_is_test.cc)
target_link_libraries(NttWithISTest gtest_main)
gtest_discover_tests(NttWithISTest)

add_executable(MinCostFlowTest mincost_flow_test.cc)
target_link_libraries(MinCostFlowTest gtest_main)
gtest_discover_tests(MinCostFlowTest)

add_executable(MinCostFlowPotentialTest mincost_flow_potential_test.cc)
target_link_libraries(MinCostFlowPotentialTest gtest_main)
gtest_discover_tests(MinCostFlowPotentialTest)

add_executable(SlopeTrickTest slope_trick_test.cc)
target_link_libraries(SlopeTrickTest gtest_main)
gtest_discover_tests(SlopeTrickTest)

add_executable(AhoCorasickTest aho_corasick_test.cc)
target_link_libraries(AhoCorasickTest gtest_main)
gtest_discover_tests(AhoCorasickTest)
